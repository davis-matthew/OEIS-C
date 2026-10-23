/*
 * A167371  Triangle, read by rows, given by [0,1,-1,0,0,0,0,0,0,0,0,...]
 *          DELTA [1,0,-1,1,0,0,0,0,0,0,0,...] where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A167371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n = r(r+1)/2 + r or n = r(r+1)/2 + r – 1 for some r
 *            ≥ 0, otherwise a(n) = 0. Here r is the row index of the infinite
 *            triangle T(n,k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the term a(n) of OEIS A167371. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       Use binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0;
    idx_t high = 1;

    /* Exponentially grow high until it passes the interval. */
    while (( (__uint128_t)high * (high + 1) / 2 ) <= n) {
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__uint128_t)mid * (mid + 1) / 2 ) <= n)
            low = mid;
        else
            high = mid;
    }

    idx_t r = low;
    /* Row offset: number of elements before row r. */
    idx_t offset = (idx_t)(( (__uint128_t)r * (r + 1) ) / 2);
    idx_t col    = n - offset;          /* column within the row (0‑based) */

    /* Triangle entry: 1 only at columns r‑1 and r (except row 1
       has only column r = 1; row 0 has only column 0). */
    if (col == r)                     return (seq_t)1;
    if (r >= 2 && col == r - 1)       return (seq_t)1;
    return (seq_t)0;
}
