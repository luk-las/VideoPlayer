#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    vw = new QVideoWidget;
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);

    slider = new QSlider(Qt::Horizontal, this);
    bar = new QProgressBar(this);
    ui->statusBar->addPermanentWidget(slider);
    ui->statusBar->addPermanentWidget(bar);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider ->setRange(0,100);
    volumeSlider ->setFixedWidth(100);
    volumeSlider ->setValue(100);
    ui->mainToolBar->addWidget(volumeSlider);

    connect(player, &QMediaPlayer::durationChanged, slider, &QSlider::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, slider, &QSlider::setValue);
    connect(slider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    connect(player, &QMediaPlayer::durationChanged, bar, &QProgressBar::setMaximum);
    connect(player, &QMediaPlayer::positionChanged, bar, &QProgressBar::setValue);

    connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a File","", "Video File (*.mp4 *.avi *.mpg)");
    on_actionStop_triggered();

    player->setMedia(QUrl::fromLocalFile(filename));
    on_actionPlay_triggered();
}

void MainWindow::on_actionPlay_triggered()
{
    player->play();
    ui->statusBar->showMessage("Playing");
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    ui->statusBar->showMessage("Paused...");
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusBar->showMessage("Stopped");
}


void MainWindow::on_actionScreen_triggered()
{
    if(this->isMaximized()==false)
    {
        this->showMaximized();
        ui->actionScreen->setIcon(QIcon(":/images/icons/minimize.png"));
    }
    else
    {
        this->showNormal();
        ui->actionScreen->setIcon(QIcon(":/images/icons/maximize.png"));
    }

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    on_actionScreen_triggered();
    e->accept();
}

int MainWindow::volume()
{
    return volumeSlider->value();
}

void MainWindow::setVolume(int volume)
{
    player->setVolume(volume);
}


void MainWindow::on_actionVolumeMin_triggered()
{
    setVolume(0);
    volumeSlider->setValue(0);
}
