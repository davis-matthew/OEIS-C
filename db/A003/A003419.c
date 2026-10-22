/*
 * A003419  Values of m in the discriminant D = 4*m leading to a new minimum
 *          of the L-function of the Dirichlet series L(1) = Sum_{k>0}
 *          Kronecker(D,k)/k.
 * https://oeis.org/A003419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over m = 2, 3, ...: compute L(1) = Sum_{k>0}
 *            Kronecker(4m,k)/k (an Euler-product prefilter, then an exact
 *            series for the fundamental discriminant times the Euler factors
 *            for the conductor) and record each m giving a new minimum; a(1)
 *            = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   e1_fn, kron_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/e1_fn.h"
#include "../utils/kron_ll.h"

#define LIM_P 2000

static const double PI_C = 3.14159265358979323846;

static int kr_init = 0;
static int np = 0;
static long long prm[400];
static signed char *qtab[400];
static double lplus[400], lminus[400];
static int s1end = 0, s2end = 0;

static uint64_t *recs = NULL;
static idx_t nrec = 0, caprec = 0;
static uint64_t curm = 2;
static int have_min = 0;
static double minL = 0.0, logmin = 0.0;

/* exact L(1, Kronecker(4m, .)); returns -1 if divergent (m square) */
static double accurate_L(uint64_t m) {
    uint64_t t = m, m0 = 1, fp = 1, p;
    for (p = 2; p * p <= t; p++) {
        int e = 0;
        while (t % p == 0) { t /= p; e++; }
        if (e & 1) m0 *= p;
        { int k; for (k = 0; k < e / 2; k++) fp *= p; }
    }
    if (t > 1) m0 *= t;
    if (m0 == 1) return -1.0;
    long long D0;
    uint64_t f;
    if (m0 % 4 == 1) { D0 = (long long)m0; f = 2 * fp; }
    else { D0 = 4 * (long long)m0; f = fp; }
    double sq = sqrt((double)D0);
    double c = sqrt(PI_C / (double)D0);
    double sum = 0.0;
    long long n;
    for (n = 1;; n++) {
        double x2 = PI_C * (double)n * (double)n / (double)D0;
        if (x2 > 45.0) break;
        int ch = kron_ll(D0, n);
        if (!ch) continue;
        double term = erfc((double)n * c) / (double)n + e1_fn(x2) / sq;
        sum += ch * term;
    }
    uint64_t g = f;
    for (p = 2; p * p <= g; p++) {
        if (g % p == 0) {
            while (g % p == 0) g /= p;
            sum *= 1.0 - (double)kron_ll(D0, (long long)p) / (double)p;
        }
    }
    if (g > 1) sum *= 1.0 - (double)kron_ll(D0, (long long)g) / (double)g;
    return sum;
}

static void init_tables(void) {
    char *sv = (char *)calloc(LIM_P, 1);
    long long i, j;
    for (i = 2; i < LIM_P; i++) {
        if (sv[i]) continue;
        for (j = i * i; j < LIM_P; j += i) sv[j] = 1;
        if (i == 2) continue;
        prm[np] = i;
        signed char *tb = (signed char *)malloc((size_t)i);
        for (j = 0; j < i; j++) tb[j] = -1;
        tb[0] = 0;
        for (j = 1; j < i; j++) tb[(j * j) % i] = 1;
        qtab[np] = tb;
        lplus[np] = -log(1.0 - 1.0 / (double)i);
        lminus[np] = -log(1.0 + 1.0 / (double)i);
        np++;
        if (i <= 31) s1end = np;
        if (i < 200) s2end = np;
    }
    free(sv);
    kr_init = 1;
}

static double add_range(uint64_t m, int lo, int hi) {
    double s = 0.0;
    int i;
    for (i = lo; i < hi; i++) {
        int ch = qtab[i][m % (uint64_t)prm[i]];
        if (ch > 0) s += lplus[i];
        else if (ch < 0) s += lminus[i];
    }
    return s;
}

static void push_rec(uint64_t v) {
    if (nrec == caprec) {
        caprec = caprec ? caprec * 2 : 32;
        recs = (uint64_t *)realloc(recs, (size_t)caprec * sizeof(uint64_t));
    }
    recs[nrec++] = v;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!kr_init) {
        init_tables();
        push_rec(1);
    }
    const double ls1 = log(0.5), ls2 = log(0.8), ls3 = log(0.92);
    while (nrec < n) {
        uint64_t m = curm++;
        double s = add_range(m, 0, s1end);
        if (have_min && s + ls1 >= logmin) continue;
        s += add_range(m, s1end, s2end);
        if (have_min && s + ls2 >= logmin) continue;
        s += add_range(m, s2end, np);
        if (have_min && s + ls3 >= logmin) continue;
        double L = accurate_L(m);
        if (L <= 0.0) continue;
        if (!have_min || L < minL) {
            have_min = 1;
            minL = L;
            logmin = log(L);
            push_rec(m);
        }
    }
    return (seq_t)recs[n - 1];
}
