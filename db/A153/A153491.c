/*
 * A153491  Triangle T(n,m)= 11*binomial(n,k) - 8 read by rows, 0<=k<=n.
 * https://oeis.org/A153491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 11 * binomial(r, k) - 8 where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index and k = n - r*(r+1)/2
 *            is the offset within that row
 * Range:     u8 n <= 30, u16 n <= 126, u32 n <= 540, u64 n <= 1034,
 *            u128 n <= 1034, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;
    /* Expand the search interval while the upper bound is too small. */
    while ((__int128)high * (high + 1) / 2 <= n) {
        /* Guard against overflow – for the problem sizes this never happens. */
        high <<= 1;
    }
    /* Binary search for the smallest r with start > n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;
    /* Index of the first element of row r. */
    idx_t start = (__int128)r * (r + 1) / 2;
    /* Position within the row. */
    idx_t k = n - start;

    seq_t coeff = binom(r, k);          /* 11 * C(r,k) - 8 */
    return (seq_t)11 * coeff - (seq_t)8;
}
