/*
 * A082771  Triangular array, read by rows: T(n,k) = Sum_{d|n} d^k, 0 <= k <
 *          n.
 * https://oeis.org/A082771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, k) where T(r,k) = sum_{d|r} d^k. r is the row number
 *            such that T(r,0),T(r,1),...,T(r,r-1) are the terms indexed from
 *            triangular number T_{r-1}+1 up to T_r. k is the column index
 *            within that row, 0‑based
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   divisor_power_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_power_sum.h"

/* Main entry point.  Computes the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* outside of sequence offset; define as 0 */
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n.
       Use 128‑bit arithmetic to avoid overflow for large n. */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Compute k = n - T_{r-1} - 1  (0‑based column index). */
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = n - prev - 1;

    return divisor_power_sum(r, k);
}
