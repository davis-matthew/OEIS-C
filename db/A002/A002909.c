/*
 * A002909  Low temperature energy function for square lattice.
 * https://oeis.org/A002909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f.: (1+x)/(1-x) + ((1-6*x+x^2)/(1-x^2))*Sum_{k>=0} (2*k)!^2 *
 *            (x*(1-x)^2/(1+x)^4)^k/k!^4; a(n) = [n==0 ? 1 : 2] + Sum_{m=0..n}
 *            p(n-m)*t(m), p(0)=1, p(j)=2-4*j^2, t(m) = Sum_{k=1..m/2}
 *            binomial(2k,k)^2*binomial(m+2k-1,4k-1) (t(0)=1), with t computed
 *            by its P-recursive recurrence for large n
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 28, i128 n <= 54,
 *            big any n
 * Helpers:   a2909_from_wrap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2909_from_wrap.h"

static unsigned __int128 a2909_C[112][112];
static int a2909_ready = 0;

static void a2909_build(void) {
    int i, j;
    for (i = 0; i < 112; i++) {
        a2909_C[i][0] = 1;
        for (j = 1; j <= i; j++)
            a2909_C[i][j] = a2909_C[i - 1][j - 1] + (j <= i - 1 ? a2909_C[i - 1][j] : 0);
    }
    a2909_ready = 1;
}

/* exact for n <= 54 using arithmetic modulo 2^128 (division-free) */
static seq_t a2909_small(idx_t n) {
    unsigned __int128 t[55];
    /* coefficient of x^n in (1+x)/(1-x) */
    unsigned __int128 acc = (n == 0) ? 1 : 2;
    idx_t m, k;
    if (!a2909_ready) a2909_build();
    t[0] = 1;
    for (m = 1; m <= n; m++) {
        unsigned __int128 s = 0;
        for (k = 1; 2 * k <= m; k++) {
            unsigned __int128 sk = a2909_C[2 * k][k] * a2909_C[2 * k][k];
            s += sk * a2909_C[m + 2 * k - 1][4 * k - 1];
        }
        t[m] = s;
    }
    for (m = 0; m <= n; m++) {
        idx_t j = n - m;
        unsigned __int128 p = (j == 0) ? (unsigned __int128)1
                                       : (unsigned __int128)2 - (unsigned __int128)(4 * j * j);
        acc += p * t[m];
    }
    return a2909_from_wrap(acc);
}

seq_t a(idx_t n) {
    seq_t t0 = 0, t1 = 0, t2 = 0, t3 = 0, tm = 1, acc;
    idx_t m;
    if (n <= 54) return a2909_small(n);
    acc = 2; /* coefficient of x^n in (1+x)/(1-x), n >= 1 */
    for (m = 0;; m++) {
        idx_t j = n - m;
        seq_t pj;
        if (j == 0) {
            pj = 1;
        } else {
            seq_t jj = (seq_t)j;
            pj = (seq_t)2 - jj * jj * 4;
        }
        acc += pj * tm;
        if (m == n) break;
        {
            long long M = (long long)m;
            seq_t num = tm * (seq_t)(7 * M * M + 2 * M)
                      - t3 * (seq_t)(6 * M * M - 30 * M + 8)
                      - t2 * (seq_t)(6 * M * M - 6 * M - 28)
                      + t1 * (seq_t)(7 * M * M - 44 * M + 69)
                      - t0 * (seq_t)((M - 4) * (M - 4));
            seq_t d = (seq_t)((M + 1) * (M + 1));
            seq_t nx;
            if (d == 0) d = 1;
            nx = num / d;
            t0 = t1; t1 = t2; t2 = t3; t3 = tm; tm = nx;
        }
    }
    return acc;
}
