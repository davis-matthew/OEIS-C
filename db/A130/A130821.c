/*
 * A130821  2n-1 appears 2n times.
 * https://oeis.org/A130821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2k-1 where k is the smallest integer with k*(k+1) >= n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(n) = A131507(ceil(n/2)-1) with offset 1 */

#include <stdint.h>

/* Find the smallest k such that k*(k+1) >= n */
static inline idx_t find_k(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Expand high until it satisfies the condition */
    while (1) {
        unsigned __int128 prod = (__int128)high * (high + 1);
        if (prod >= n) break;
        if (high > (UINT64_MAX >> 1)) { /* guard against overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (__int128)mid * (mid + 1);
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behavior below offset not required */

    idx_t k = find_k(n);
    /* compute 2*k-1 in seq_t to avoid overflow of idx_t */
    seq_t two_k = (seq_t)2 * (seq_t)k;
    return two_k - (seq_t)1;
}
