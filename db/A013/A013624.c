/*
 * A013624  Triangle of coefficients in expansion of (3+7x)^n.
 * https://oeis.org/A013624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(r,k) * 7^k * 3^(r-k) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that the triangular number T(r) = r*(r+1)/2
       is the largest triangular number <= n.  We work with idx_t
       (uint64_t) and no overflow occurs for the ranges used in the tests. */
    idx_t r = 0;
    while ((r + 1u) * (r + 2u) / 2u <= n)
        ++r;

    /* Cumulative number of terms up to row r-1.  We cast to seq_t to avoid
       overflow in intermediate 64‑bit multiplication if necessary. */
    seq_t tri = (seq_t)r * ((seq_t)r + 1u) / 2u;

    /* Column index within the row.  Since tri <= n < tri + r + 1,
       (n - tri) fits in idx_t. */
    idx_t k = (idx_t)(((seq_t)n) - tri);

    /* Compute powers of 7 and 3 up to the required exponents.  The
       exponent k is at most r, and r will be far smaller than the limits
       of seq_t for all values of n that appear in the tests. */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow7 *= (seq_t)7u;

    seq_t pow3 = 1;
    for (idx_t i = 0; i < r - k; ++i)
        pow3 *= (seq_t)3u;

    /* Compute binomial coefficient using the provided utility. */
    seq_t coeff = binomial(r, k);

    /* Combine everything.  All operations are on seq_t, which supports
       arbitrary‑precision arithmetic when needed. */
    coeff *= pow7;
    coeff *= pow3;

    return coeff;
}
