/*
 * A184179  Number of permutations of {1,2,...,n} having no isolated fixed
 *          points. A fixed point j of a permutation is said to be isolated if
 *          neither j-1 nor j+1 is a fixed point. For example, 4135267 has
 *          only 3 as an isolated fixed point.
 * https://oeis.org/A184179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} d(n-j) * Σ_{m=0..⌊j/2⌋} binomial(j-m-1,m-1) *
 *            * binomial(n+1-j,m) * where d(i) = A000166(i) (derangement
 *            numbers)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Computes the OEIS sequence A184179: number of permutations
 * of {1,…,n} having no isolated fixed points. */
seq_t a(idx_t n)
{
    /* derangements d[0..n] */
    seq_t *d = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    d[0] = (seq_t)1;
    if (n >= 1)
        d[1] = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        /* d(i) = (i-1)*(d(i-1)+d(i-2)) */
        d[i] = (seq_t)(i - 1) * (d[i - 1] + d[i - 2]);
    }

    seq_t res = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t inner = (seq_t)0;
        /* inner sum over m = 0 .. floor(j/2) */
        for (idx_t m = 0; m <= j / 2; ++m) {
            seq_t b1 = binomial(j - m - 1, m - 1);
            seq_t b2 = binomial(n + 1 - j, m);
            inner += b1 * b2;
        }
        res += d[n - j] * inner;
    }

    free(d);
    return res;
}
