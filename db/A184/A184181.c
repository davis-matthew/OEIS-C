/*
 * A184181  Number of permutations of {1,2,...,n} whose shortest block is of
 *          length 1. A block of a permutation is a maximal sequence of
 *          consecutive integers which appear in consecutive positions. For
 *          example, the permutation 5412367 has 4 blocks: 5, 4, 123, and 67.
 *          Its shortest block has length 1.
 * https://oeis.org/A184181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1..n} C(n‑1, m‑1)·(d(m)+d(m‑1)) – Σ_{m=1..⌊n/2⌋}
 *            C(n‑m‑1, m‑1)·(d(m)+d(m‑1)) where d(j) is the derangement number
 *            A000166(j)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 0. */
    if (n == 0) return 0;

    /* Allocate an array for derangement numbers d[0..n]. */
#ifdef __cplusplus
    std::vector<seq_t> d(static_cast<size_t>(n + 1));
#else
    seq_t *d = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!d) abort(); /* allocation failed */
#endif

    /* Base cases for derangement numbers. */
    d[0] = 1;        /* d(0) = 1 */
    if (n >= 1) d[1] = 0;   /* d(1) = 0 */

    /* Compute derangements up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* d[i] = (i-1) * (d[i-1] + d[i-2]); */
#ifdef __cplusplus
        d[i] = (seq_t)(i - 1) * (d[i - 1] + d[i - 2]);
#else
        d[i] = (seq_t)(i - 1) * (d[i - 1] + d[i - 2]);
#endif
    }

    seq_t result = 0;

    /* First sum: m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t term = binomial(n - 1, m - 1);        /* C(n-1, m-1) */
        term *= (d[m] + d[m - 1]);                  /* * (d(m)+d(m-1)) */
        result += term;
    }

    /* Second sum: m = 1 .. ⌊n/2⌋ */
    idx_t half = n / 2;
    for (idx_t m = 1; m <= half; ++m) {
        seq_t term = binomial(n - m - 1, m - 1);    /* C(n-m-1, m-1) */
        term *= (d[m] + d[m - 1]);                  /* * (d(m)+d(m-1)) */
        result -= term;
    }

#ifndef __cplusplus
    free(d);
#endif

    return result;
}
