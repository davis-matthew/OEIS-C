/*
 * A000456  Number of permutations of [n] in which the longest increasing run
 *          has length 5.
 * https://oeis.org/A000456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f_5(n) - f_4(n), where f_k(n) counts the permutations of
 *            [n] whose increasing runs all have length <= k. E.g.f. of f_k is
 *            1/Sum_{j>=0} (x^(j(k+1))/(j(k+1))! - x^(j(k+1)+1)/(j(k+1)+1)!),
 *            which gives f_k(0) = 1, f_k(n) = Sum_{m==1 mod (k+1)} C(n,m)
 *            f_k(n-m) - Sum_{m>0, m==0 mod (k+1)} C(n,m)...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_arr.h"

static seq_t *P = NULL;   /* Pascal row M */
static seq_t *F4 = NULL;
static seq_t *F5 = NULL;
static idx_t cap = 0;
static idx_t M = 0;

static seq_t step(const seq_t *F, idx_t i, idx_t k1) {
    seq_t pos = 0, neg = 0;
    idx_t m;
    for (m = 1; m <= i; m += k1) pos += P[m] * F[i - m];
    for (m = k1; m <= i; m += k1) neg += P[m] * F[i - m];
    return pos - neg;
}

seq_t a(idx_t n) {
    idx_t i, j;
    if (cap == 0) {
        cap = 64;
        P = alloc_zero(cap);
        F4 = alloc_zero(cap);
        F5 = alloc_zero(cap);
        for (i = 0; i < cap; i++) { P[i] = 0; F4[i] = 0; F5[i] = 0; }
        P[0] = 1;
        F4[0] = 1;
        F5[0] = 1;
        M = 0;
    }
    if (n + 1 > cap) {
        idx_t nc = cap;
        while (nc < n + 1) nc *= 2;
        P = grow_arr(P, cap, nc);
        F4 = grow_arr(F4, cap, nc);
        F5 = grow_arr(F5, cap, nc);
        for (i = cap; i < nc; i++) { P[i] = 0; F4[i] = 0; F5[i] = 0; }
        cap = nc;
    }
    for (i = M + 1; i <= n; i++) {
        P[i] = 0;
        for (j = i; j >= 1; j--) P[j] += P[j - 1];
        F4[i] = step(F4, i, 5);
        F5[i] = step(F5, i, 6);
        M = i;
    }
    return F5[n] - F4[n];
}
