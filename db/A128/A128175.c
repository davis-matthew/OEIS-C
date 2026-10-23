/*
 * A128175  Binomial transform of A128174.
 * https://oeis.org/A128175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A128175 – the flattened triangle A128174.
    Each term a(n) is the (r,k)th entry of the triangle T, where
          r*(r-1)/2 < n ≤ r*(r+1)/2   and   k = n – r*(r-1)/2.
    The triangle entry is
          T(r,k) = Σ_{i=0..⌊(r-k)/2⌋} binom(r-1, k-1+2*i).
    The binomial coefficient is obtained from the helper function
    binom() provided in the utils library.                                   */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the n‑th element in the flattened
       triangle.  The cumulative number of elements up to row r is
       r*(r+1)/2.  We increment r until this exceeds or equals n.     */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position k of the element within row r (1‑based).            */
    idx_t k = n - (cum - r);

    /* Compute T(r,k) = Σ binom(r-1, k-1+2*i) for i = 0..⌊(r-k)/2⌋. */
    seq_t sum = 0;
    idx_t max_i = (r > k) ? (r - k) / 2 : 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t j = k - 1 + 2 * i;          /* the binomial pick index   */
        sum += binom(r - 1, j);          /* binom is seq_t returning */
    }
    return sum;
}
