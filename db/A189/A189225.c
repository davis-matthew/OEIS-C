/*
 * A189225  Entries in a 4-dimensional version of Pascal's triangle:
 *          quadrinomial coefficients of (a + b + c + d)^r.
 * https://oeis.org/A189225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = binomial(r,p) * binomial(p,m) * binomial(m,k) where
 *            (r,p,m,k) are obtained from the index n by n = binomial(r+3,4) +
 *            binomial(p+2,2) + binomial(m+1,2) + k with 0 ≤ k ≤ m ≤ p ≤ r
 * Range:     u8 n <= 34, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 1. Find the slice index r such that binom(r+3,4) ≤ n < binom(r+4,4) */
    idx_t r = 0;
    while (binomial(r + 4, 4) <= n)
        ++r;

    /* 2. Offset inside that slice */
    seq_t base = binomial(r + 3, 4);
    seq_t offset = n - base;                /* offset is still seq_t */

    /* 3. Find p inside the slice: subtract block sizes binom(p+2,2) */
    idx_t p = 0;
    for (;; ++p) {
        seq_t block = binomial(p + 2, 2);   /* (p+1)(p+2)/2 terms for this p */
        if (offset < block)
            break;
        offset -= block;
    }

    /* 4. Find m inside the chosen p block: subtract (m+1) terms */
    idx_t m = 0;
    for (;; ++m) {
        seq_t block = (seq_t)(m + 1);       /* number of k values for this m */
        if (offset < block)
            break;
        offset -= block;
    }

    /* 5. Remaining offset is k */
    idx_t k = (idx_t)offset;

    /* 6. Compute the quadrinomial coefficient */
    seq_t val = binomial(r, p);
    val *= binomial(p, m);
    val *= binomial(m, k);
    return val;
}
