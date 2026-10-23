/*
 * A137260  Triangle T(n, k) = k*(n-1)! - k!, read by rows.
 * https://oeis.org/A137260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n * k
 *            = n - r(r-1)/2 * T(r,k) = k * (r-1)! - k!
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 86, u64 n <= 217,
 *            u128 n <= 596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the row–wise enumeration of the triangle T(n,k) = k*(n-1)! - k!. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Find the row r such that r*(r+1)/2 >= n.  Use binary search
     * with 128‑bit arithmetic to avoid overflow on intermediate products. */
    idx_t low = 1, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k = position within the row
     * cumulative terms before the row r is (r-1)*r/2. */
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k   = n - prev;

    /* Compute (r-1)! and k! using seq_t arithmetic. */
    seq_t fact_r_minus1 = 1, fact_k = 1;
    for (idx_t i = 2; i < r; ++i)          /* (r-1)!  (stop before r) */
        fact_r_minus1 *= (seq_t)i;
    for (idx_t i = 2; i <= k; ++i)          /* k!       */
        fact_k *= (seq_t)i;

    /* T(r,k) = k*(r-1)! - k! */
    seq_t result = (seq_t)k * fact_r_minus1 - fact_k;
    return result;
}
