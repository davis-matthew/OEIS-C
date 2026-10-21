/*
 * A212151  Number of 2 X 2 matrices M of positive integers such that
 *          permanent(M) < n.
 * https://oeis.org/A212151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{n-1} d(k) * Σ_{q=1}^{n-k-1} d(q) where d(x) is
 *            the divisor count of x. This is an O(n log n) implementation
 *            using a simple sieve
 * Range:     u8 n <= 11, u16 n <= 88, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 2)  /* a(0) = a(1) = a(2) = 0 */
        return 0;

    /* Allocate arrays for divisor counts and their prefix sums.
       Size n is enough because indices used run from 1 .. n-1. */
    size_t sz = (size_t)n;
    seq_t *d   = (seq_t *)calloc(sz, sizeof(seq_t)); /* divisor counts */
    seq_t *dsum = (seq_t *)malloc(sz * sizeof(seq_t)); /* prefix sums of d */

    /* Simple divisor sieve: for each i add 1 to every multiple of i. */
    for (idx_t i = 1; i < n; ++i) {
        for (idx_t j = i; j < n; j += i)
            d[j]++;   /* d[j]++ uses seq_t type automatically */
    }

    /* Prefix sums of divisor counts: dsum[m] = Σ_{q=1}^{m} d(q) */
    dsum[0] = 0;
    for (idx_t m = 1; m < n; ++m) {
        dsum[m] = dsum[m - 1] + d[m];
    }

    /* Compute the main sum: a(n) = Σ_{k=1}^{n-1} d(k) * dsum[n-k-1] */
    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t rem = n - k - 1;          /* non‑negative, 0 ≤ rem ≤ n-2 */
        result += d[k] * dsum[rem];
    }

    free(d);
    free(dsum);
    return result;
}
