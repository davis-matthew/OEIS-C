/*
 * A007252  McKay-Thompson series of class 5B for the Monster group with a(0)
 *          = 0.
 * https://oeis.org/A007252
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^(n+1)] (eta(q)/eta(q^5))^6 for n != 0, a(0) = 0. With
 *            S(q) = prod_{k>=1}(1-q^k)^6 = (sum_{m>=0} (-1)^m (2m+1)
 *            q^(m(m+1)/2))^2 (Jacobi), f(q) = S(q)/S(q^5) is computed by f[m]
 *            = S[m] - sum_{j>=1} S[j]*f[m-5j], and a(n) = f[n+1] (+6 when n =
 *            0)
 * Range:     i8 n <= 7, i16 n <= 27, i32 n <= 100, i64 n <= 367,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *S_ser = 0;
static seq_t *F_ser = 0;
static idx_t cap = 0;

static void ensure(idx_t need) {
    idx_t nc, i, m, j;
    long long m1, m2;
    if (need <= cap) return;
    nc = cap ? cap : 64;
    while (nc < need) nc *= 2;
    if (S_ser == 0) {
        S_ser = alloc_zero(nc);
        F_ser = alloc_zero(nc);
    } else {
        S_ser = grow_zero(S_ser, cap, nc);
        F_ser = grow_zero(F_ser, cap, nc);
    }
    for (i = 0; i < nc; i++) S_ser[i] = 0;
    for (m1 = 0; m1 * (m1 + 1) / 2 < nc; m1++) {
        long long t1 = m1 * (m1 + 1) / 2;
        long long c1 = (m1 & 1) ? -(2 * m1 + 1) : (2 * m1 + 1);
        for (m2 = 0; t1 + m2 * (m2 + 1) / 2 < nc; m2++) {
            long long t2 = m2 * (m2 + 1) / 2;
            long long c2 = (m2 & 1) ? -(2 * m2 + 1) : (2 * m2 + 1);
            S_ser[t1 + t2] += (seq_t)(c1 * c2);
        }
    }
    for (m = cap; m < nc; m++) {
        seq_t v = S_ser[m];
        for (j = 1; 5 * j <= m; j++) {
            v -= S_ser[j] * F_ser[m - 5 * j];
        }
        F_ser[m] = v;
    }
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t m = n + 1;
    seq_t r;
    if (m < 0) return 0;
    ensure(m + 1);
    r = F_ser[m];
    if (n == 0) r += 6;
    return r;
}
