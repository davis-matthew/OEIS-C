/*
 * A073218  The terms of A055258 (sums of two powers of 7) divided by 2.
 * https://oeis.org/A073218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (7^r + 7^k) / 2 where r = floor((sqrt(8n+1)-1)/2) (the
 *            row index) k = n - r*(r+1)/2 (the column index within that row)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of A073218.
   n is given as idx_t (uint64_t).  The result is returned as seq_t. */
seq_t a(idx_t n)
{
    /* 1. Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* 2. Compute the column index k. */
    idx_t tri = r * (r + 1) / 2;
    idx_t k = n - tri;

    /* 3. Compute powers of 7 up to 7^r. */
    seq_t *p = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!p) return 0;      /* allocation failure – return 0 as a safe default */

    p[0] = 1;
    for (idx_t i = 1; i <= r; ++i)
        p[i] = p[i - 1] * 7;

    /* 4. Compute the term: (7^r + 7^k) / 2. */
    seq_t result = (p[r] + p[(size_t)k]) / 2;

    /* 5. Clean up and return. */
    free(p);
    return result;
}
