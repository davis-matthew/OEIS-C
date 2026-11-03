/*
 * A003272  a(n) = ceiling((-4n)/Bernoulli(2n)).
 * https://oeis.org/A003272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ceiling(-4n/Bernoulli(2n)), a(0) = 0; for n >= 10,
 *            |B(2n)| > 4n, so a(n) = 1 for even n and 0 for odd n
 * Range:     i8 n <= 1, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 899 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static long long a3272_g(long long x, long long y) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y) {
        long long t = x % y;
        x = y;
        y = t;
    }
    return x ? x : 1;
}

static void a3272_norm(long long *num, long long *den) {
    long long g;
    if (*den < 0) {
        *num = -*num;
        *den = -*den;
    }
    if (*num == 0) {
        *den = 1;
        return;
    }
    g = a3272_g(*num, *den);
    *num /= g;
    *den /= g;
}

/* Exact Bernoulli number B_k for small k via sum_{j=0}^{m} C(m+1,j) B_j = 0 */
static void a3272_bern(int k, long long *num, long long *den) {
    long long bn[24], bd[24];
    int m, j;
    bn[0] = 1;
    bd[0] = 1;
    for (m = 1; m <= k; m++) {
        long long sn = 0, sd = 1, c = 1;
        for (j = 0; j < m; j++) {
            long long tn = c * bn[j], td = bd[j];
            long long g = a3272_g(sd, td);
            sn = sn * (td / g) + tn * (sd / g);
            sd = sd / g * td;
            a3272_norm(&sn, &sd);
            c = c * (m + 1 - j) / (j + 1);
        }
        bn[m] = -sn;
        bd[m] = sd * (m + 1);
        a3272_norm(&bn[m], &bd[m]);
    }
    *num = bn[k];
    *den = bd[k];
}

seq_t a(idx_t n) {
    long long bnum, bden, p, q, r;
    if (n == 0) return 0;
    if (n >= 10) return (n & 1) ? (seq_t)0 : (seq_t)1;
    a3272_bern(2 * (int)n, &bnum, &bden);
    /* value = -4n / (bnum/bden) = -4n*bden / bnum */
    p = -4LL * (long long)n * bden;
    q = bnum;
    if (q < 0) {
        p = -p;
        q = -q;
    }
    if (p >= 0)
        r = (p + q - 1) / q;
    else
        r = p / q;
    return (seq_t)r;
}
