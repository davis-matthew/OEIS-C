/*
 * A204547  Symmetric matrix: f(i,j)=floor[(i+j+4)/4]-floor[(i+j+2)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((m+4)/4)-floor((m+2)/4), where m is the smallest
 *            positive integer satisfying m(m-1)/2 >= n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_m(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 2;
    /* increase high until it big enough */
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;
    /* binary search for the minimal m */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the antidiagonal index m */
    idx_t m = find_m(n);

    /* Compute the value f(i,j) which depends only on m = i+j */
    /* Since f(i,j) = floor((m+4)/4) – floor((m+2)/4), we can compute it directly. */
    seq_t val = ((seq_t)(m + 4)) / 4 - ((seq_t)(m + 2)) / 4;
    return val;
}
