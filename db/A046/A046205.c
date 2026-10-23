/*
 * A046205  In Leibniz's Harmonic Triangle, write numerator first and then
 *          denominator of each element.
 * https://oeis.org/A046205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy, less
 * Formula:   a(n) = 1 if n is odd; otherwise let m = n/2. * Find the row r
 *            such that r(r+1)/2 ≥ m; let j = m - r(r-1)/2. * Then a(n) = r *
 *            binom(r-1, j-1), where binom is the binomial coefficient
 * Range:     u8 n <= 63, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   binom_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_ll.h"

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == (idx_t)1)  /* odd indices have numerator 1 */
        return (seq_t)1;

    idx_t m = n >> 1;                /* n = 2m: the element index in the triangle */

    /* Find the row r that contains element m */
    idx_t r = (idx_t)0;
    while (r * (r + (idx_t)1) / (idx_t)2 < m)
        ++r;

    idx_t prev = r * (r - (idx_t)1) / (idx_t)2;  /* elements before row r */
    idx_t j = m - prev;                          /* position within row r (1‑based) */

    seq_t comb = binom_ll(r - (idx_t)1, j - (idx_t)1);
    return (seq_t)r * comb;
}
