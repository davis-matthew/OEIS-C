/*
 * A000467  Number of permutations of [n] in which the longest increasing run
 *          has length 6.
 * https://oeis.org/A000467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F_6(n) - F_5(n), where F_k(n) counts permutations of [n]
 *            whose increasing runs all have length <= k. The e.g.f. of F_k is
 *            1/Sum_{j>=0} (x^(j(k+1))/(j(k+1))! - x^(j(k+1)+1)/(j(k+1)+1)!).
 *            This gives F_k(0) = 1 and F_k(n) = Sum_{m==1 mod (k+1)} C(n,m)
 *            F_k(n-m) - Sum_{m==0 mod (k+1), m>0} C(n,...
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_arr.h"

static seq_t *F5 = 0, *F6 = 0, *row = 0;
static idx_t have = 0, cap = 0;

static seq_t step(const seq_t *F, idx_t i, idx_t p)
{
    seq_t pos = 0, neg = 0;
    idx_t m;
    for (m = 1; m <= i; m += p)
        pos += row[m] * F[i - m];
    for (m = p; m <= i; m += p)
        neg += row[m] * F[i - m];
    return pos - neg;
}

static void extend(idx_t N)
{
    idx_t i, m;
    if (N + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < N + 1) nc *= 2;
        F5 = grow_arr(F5, cap, nc);
        F6 = grow_arr(F6, cap, nc);
        row = grow_arr(row, cap, nc);
        cap = nc;
    }
    for (i = have; i <= N; i++) {
        /* update row to Pascal row i */
        row[i] = 1;
        if (i > 0)
            for (m = i - 1; m >= 1; m--)
                row[m] += row[m - 1];
        if (i == 0) {
            F5[0] = 1;
            F6[0] = 1;
        } else {
            F5[i] = step(F5, i, 6);
            F6[i] = step(F6, i, 7);
        }
    }
    if (N + 1 > have) have = N + 1;
}

seq_t a(idx_t n)
{
    if (n + 1 > have) extend(n);
    return F6[n] - F5[n];
}
