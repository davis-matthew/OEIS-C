/*
 * A199512  Triangle T(n,k) = Fibonacci(n+k+1), related to A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A199512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Fib( r + k + 1 ) where r = floor((sqrt(8*n + 1) - 1) / 2)
 *            (row index) k = n - r*(r + 1)/2 (position within that row)
 * Range:     u8 n <= 33, u16 n <= 89, u32 n <= 322, u64 n <= 1173,
 *            u128 n <= 1325, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Find the row r such that  r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
static inline idx_t row_of(idx_t n)
{
    /* exponential search to find an upper bound */
    idx_t low  = 0;
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* binary search within [low, high] to get largest r with r*(r+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r     = row_of(n);                     /* row index */
    idx_t start = r * (r + 1) / 2;                /* first index of this row */
    idx_t k     = n - start;                      /* offset within the row */
    idx_t fidx  = r + k + 1;                      /* Fibonacci index to use   */
    return fib(fidx);
}
