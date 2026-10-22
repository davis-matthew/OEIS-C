/*
 * A002874  The number of partitions of {1..3n} that are invariant under a
 *          permutation consisting of n 3-cycles.
 * https://oeis.org/A002874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   E.g.f. exp((exp(3*x) - 4)/3 + exp(x)); taking the derivative
 *            gives a(0) = 1, a(m+1) = Sum_{k=0..m} binomial(m,k) * (3^(m-k) +
 *            1) * a(k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *A_f = 0;   /* a(0..have) */
static seq_t *A_r = 0;   /* binomial row 'have' */
static seq_t *A_g = 0;   /* 3^j + 1 for j < have */
static seq_t A_pow3;
static idx_t A_have = 0, A_cap = 0;

seq_t a(idx_t n)
{
    if (A_cap == 0) {
        A_cap = 16;
        A_f = A_grow(0, 0, A_cap);
        A_r = A_grow(0, 0, A_cap);
        A_g = A_grow(0, 0, A_cap);
        A_f[0] = 1;
        A_r[0] = 1;
        A_pow3 = 1;
        A_have = 0;
    }
    if (n + 1 > A_cap) {
        idx_t nc = A_cap;
        while (nc < n + 1) nc *= 2;
        A_f = A_grow(A_f, A_cap, nc);
        A_r = A_grow(A_r, A_cap, nc);
        A_g = A_grow(A_g, A_cap, nc);
        A_cap = nc;
    }
    while (A_have < n) {
        idx_t m = A_have, k;
        seq_t sum = 0;
        A_g[m] = A_pow3 + 1;
        A_pow3 = A_pow3 * 3;
        for (k = 0; k <= m; k++)
            sum += A_r[k] * A_g[m - k] * A_f[k];
        A_f[m + 1] = sum;
        /* advance binomial row from m to m+1 */
        A_r[m + 1] = 1;
        for (k = m; k >= 1; k--)
            A_r[k] += A_r[k - 1];
        A_have = m + 1;
    }
    return A_f[n];
}
