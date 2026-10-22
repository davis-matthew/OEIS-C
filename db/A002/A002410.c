/*
 * A002410  Nearest integer to imaginary part of n-th zero of Riemann zeta
 *          function.
 * https://oeis.org/A002410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = round(t_n), where t_n is the n-th positive zero of the
 *            Riemann-Siegel Z(t) (search: scan Z(t) =
 *            2*Sum_{k<=sqrt(t/(2*Pi))} cos(theta(t) - t*log k)/sqrt(k) + R(t)
 *            for sign changes, refine by bisection, golden search to catch
 *            close pairs)
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define RZ_PI 3.14159265358979323846

static double rz_theta(double t)
{
    return t / 2.0 * log(t / (2.0 * RZ_PI)) - t / 2.0 - RZ_PI / 8.0
         + 1.0 / (48.0 * t) + 7.0 / (5760.0 * t * t * t);
}

static double rz_Z(double t)
{
    double tau = t / (2.0 * RZ_PI);
    double sq = sqrt(tau);
    long long N = (long long)floor(sq);
    double p = sq - (double)N;
    double th = rz_theta(t);
    double sum = 0.0, c, C0, R;
    long long k;
    for (k = 1; k <= N; k++)
        sum += cos(th - t * log((double)k)) / sqrt((double)k);
    c = cos(2.0 * RZ_PI * p);
    if (fabs(c) < 1e-9) {
        p += 1e-7;
        c = cos(2.0 * RZ_PI * p);
    }
    C0 = cos(2.0 * RZ_PI * (p * p - p - 1.0 / 16.0)) / c;
    R = pow(tau, -0.25) * C0;
    if ((N - 1) % 2 != 0) R = -R;
    return 2.0 * sum + R;
}

static double rz_bisect(double lo, double hi)
{
    double flo = rz_Z(lo);
    int i;
    for (i = 0; i < 60; i++) {
        double mid = 0.5 * (lo + hi);
        double fm = rz_Z(mid);
        if ((fm < 0) == (flo < 0)) { lo = mid; flo = fm; }
        else hi = mid;
    }
    return 0.5 * (lo + hi);
}

static double *rz_zeros = NULL;
static size_t rz_cnt = 0, rz_cap = 0;
static int rz_init = 0;
static double rz_tpp, rz_zpp, rz_tp, rz_zp;
static int rz_havepp = 0;

static void rz_push(double z)
{
    if (rz_cnt == rz_cap) {
        size_t nc = rz_cap ? rz_cap * 2 : 1024;
        rz_zeros = (double *)realloc(rz_zeros, nc * sizeof(double));
        rz_cap = nc;
    }
    rz_zeros[rz_cnt++] = z;
}

static double rz_step(double t)
{
    double sp = 2.0 * RZ_PI / log(t / (2.0 * RZ_PI)) / 16.0;
    if (sp > 0.1 || !(sp > 0)) sp = 0.1;
    return sp;
}

static void rz_extend(size_t need)
{
    if (!rz_init) {
        rz_tp = 10.0;
        rz_zp = rz_Z(rz_tp);
        rz_havepp = 0;
        rz_init = 1;
    }
    while (rz_cnt < need) {
        double t1 = rz_tp + rz_step(rz_tp);
        double z1 = rz_Z(t1);
        if ((z1 < 0) != (rz_zp < 0)) {
            rz_push(rz_bisect(rz_tp, t1));
        } else if (rz_havepp && (rz_zpp < 0) == (rz_zp < 0)
                   && fabs(rz_zp) < fabs(rz_zpp) && fabs(rz_zp) < fabs(z1)) {
            /* possible hidden pair of close zeros: minimize s*Z on [tpp, t1] */
            double s = (rz_zp < 0) ? -1.0 : 1.0;
            double a0 = rz_tpp, b0 = t1;
            double gr = 0.6180339887498949;
            double x1 = b0 - gr * (b0 - a0), x2 = a0 + gr * (b0 - a0);
            double f1 = s * rz_Z(x1), f2 = s * rz_Z(x2);
            double found = -1.0;
            int i;
            if (f1 < 0) found = x1;
            else if (f2 < 0) found = x2;
            for (i = 0; i < 60 && found < 0; i++) {
                if (f1 < f2) {
                    b0 = x2; x2 = x1; f2 = f1;
                    x1 = b0 - gr * (b0 - a0);
                    f1 = s * rz_Z(x1);
                    if (f1 < 0) found = x1;
                } else {
                    a0 = x1; x1 = x2; f1 = f2;
                    x2 = a0 + gr * (b0 - a0);
                    f2 = s * rz_Z(x2);
                    if (f2 < 0) found = x2;
                }
            }
            if (found >= 0) {
                rz_push(rz_bisect(rz_tpp, found));
                rz_push(rz_bisect(found, t1));
            }
        }
        rz_tpp = rz_tp; rz_zpp = rz_zp;
        rz_tp = t1; rz_zp = z1;
        rz_havepp = 1;
    }
}

seq_t a(idx_t n)
{
    double t;
    if (n < 1) return (seq_t)0;
    rz_extend((size_t)n);
    t = rz_zeros[n - 1];
    return (seq_t)(uint64_t)floor(t + 0.5);
}
