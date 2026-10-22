/*
 * A003420  Values of m in the discriminant D = -4*m leading to a new maximum
 *          of the L-function of the Dirichlet series L(1) = Sum_{k=1..oo}
 *          Kronecker(D,k)/k.
 * https://oeis.org/A003420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over m = 1, 2, 3, ...: L(1) for D = -4*m equals
 *            2*Pi*h(D)/(w(D)*sqrt(4*m)), where h(D) is the class number of
 *            primitive forms of discriminant D, w(-4) = 4 and w = 2
 *            otherwise. a(n) is the n-th m at which h(D)^2/(w(D)^2*m) reaches
 *            a new strict maximum, compared exactly with integers. A trun...
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   k3420_jac, k3420_kron
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/k3420_jac.h"
#include "../utils/k3420_kron.h"

static const double K3420_PI = 3.14159265358979323846;

/* class number of discriminant -4m (primitive forms) */
static uint64_t k3420_h(uint64_t m) {
    uint64_t rem = m, s = 1, t = 1, p;
    for (p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e & 1) s *= p;
        for (int i = 0; i < e / 2; i++) t *= p;
    }
    if (rem > 1) s *= rem;
    long long d0; uint64_t f;
    if ((s & 3) == 3) { d0 = -(long long)s; f = 2 * t; }
    else { d0 = -4 * (long long)s; f = t; }
    uint64_t ad = (uint64_t)(-d0);
    int w0 = (d0 == -3) ? 6 : (d0 == -4 ? 4 : 2);
    double c = sqrt(K3420_PI / (double)ad), sd = sqrt((double)ad) / K3420_PI, S = 0.0;
    uint64_t N = (uint64_t)(3.6 * sqrt((double)ad)) + 10, k;
    for (k = 1; k <= N; k++) {
        int ch = k3420_kron(d0, k);
        if (!ch) continue;
        double x = (double)k * c;
        S += ch * (erfc(x) + sd / (double)k * exp(-x * x));
    }
    uint64_t h = (uint64_t)llround((double)w0 / 2.0 * S);
    /* conductor factor */
    uint64_t fr = f, num = h;
    for (p = 2; p * p <= fr || fr > 1; p++) {
        if (p * p > fr) p = fr;
        if (fr % p) continue;
        int e = 0;
        while (fr % p == 0) { fr /= p; e++; }
        int ch = k3420_kron(d0, p);
        num *= (uint64_t)((long long)p - ch);
        for (int i = 1; i < e; i++) num *= p;
    }
    if (f > 1) num /= (uint64_t)(w0 / 2);
    return num;
}

static int k3420_init = 0;
static int *k3420_p1, *k3420_off, *k3420_res, k3420_n1;
static double *k3420_tab;
static int *k3420_p2, k3420_n2;
static uint64_t *k3420_rec;
static size_t k3420_cnt = 0, k3420_cap = 0;
static uint64_t k3420_cur = 0, k3420_bm = 1;
static unsigned __int128 k3420_bv = 0;
static double k3420_blog = -1e300;

static void k3420_setup(void) {
    const int LIM = 20000;
    char *comp = (char *)calloc(LIM + 1, 1);
    int i, j, tot = 0;
    k3420_p1 = (int *)malloc(sizeof(int) * LIM);
    k3420_p2 = (int *)malloc(sizeof(int) * LIM);
    k3420_n1 = k3420_n2 = 0;
    for (i = 2; i <= LIM; i++) {
        if (comp[i]) continue;
        for (j = 2 * i; j <= LIM; j += i) comp[j] = 1;
        if (i == 2) continue;
        if (i < 1000) { k3420_p1[k3420_n1++] = i; tot += i; }
        else k3420_p2[k3420_n2++] = i;
    }
    free(comp);
    k3420_off = (int *)malloc(sizeof(int) * k3420_n1);
    k3420_res = (int *)malloc(sizeof(int) * k3420_n1);
    k3420_tab = (double *)malloc(sizeof(double) * tot);
    int o = 0;
    for (i = 0; i < k3420_n1; i++) {
        int p = k3420_p1[i];
        k3420_off[i] = o;
        k3420_res[i] = 0;
        double lp = -log(1.0 - 1.0 / p), lm = -log(1.0 + 1.0 / p);
        for (j = 0; j < p; j++) {
            int q = k3420_jac((uint64_t)j, (uint64_t)p);
            k3420_tab[o + j] = q == 0 ? 0.0 : (q > 0 ? lp : lm);
        }
        o += p;
    }
    k3420_init = 1;
}

seq_t a(idx_t n) {
    if (!k3420_init) k3420_setup();
    while (k3420_cnt < (size_t)n) {
        uint64_t m = ++k3420_cur;
        double s1 = 0.0;
        int i;
        for (i = 0; i < k3420_n1; i++) {
            int r = k3420_res[i];
            r = (r == 0) ? k3420_p1[i] - 1 : r - 1;
            k3420_res[i] = r;
            s1 += k3420_tab[k3420_off[i] + r];
        }
        if (m >= 5000) {
            if (s1 < k3420_blog - 0.15) continue;
            double s2 = s1;
            for (i = 0; i < k3420_n2; i++) {
                uint64_t p = (uint64_t)k3420_p2[i];
                int q = k3420_jac((p - m % p) % p, p);
                if (q > 0) s2 += -log(1.0 - 1.0 / (double)p);
                else if (q < 0) s2 += -log(1.0 + 1.0 / (double)p);
            }
            if (s2 < k3420_blog - 0.06) continue;
        }
        uint64_t h = k3420_h(m);
        unsigned __int128 v = (m == 1) ? 1 : (unsigned __int128)4 * h * h;
        if (v * k3420_bm > k3420_bv * m) {
            k3420_bv = v;
            k3420_bm = m;
            double wD = (m == 1) ? 4.0 : 2.0;
            k3420_blog = log(K3420_PI * (double)h / (wD * sqrt((double)m)));
            if (k3420_cnt == k3420_cap) {
                size_t nc = k3420_cap ? 2 * k3420_cap : 64;
                uint64_t *nr = (uint64_t *)malloc(sizeof(uint64_t) * nc);
                if (k3420_cnt) memcpy(nr, k3420_rec, sizeof(uint64_t) * k3420_cnt);
                free(k3420_rec);
                k3420_rec = nr;
                k3420_cap = nc;
            }
            k3420_rec[k3420_cnt++] = m;
        }
    }
    return (seq_t)k3420_rec[n - 1];
}
