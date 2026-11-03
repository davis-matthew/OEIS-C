/*
 * A099375  Sequence matrix for odd numbers.
 * https://oeis.org/A099375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2*(m - k) - 1, where m = floor((sqrt(8*n + 1) + 1) / 2)
 *            (block number), and k = n - (m-1)*m/2 (offset within the block)
 * Range:     u8 n <= 8255, u16 n <= 11475, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 103 terms through n = 9408 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */

static idx_t _block_index(idx_t n)
{
    /* Find the smallest m such that m*(m+1)/2 > n.
       This is the block number containing index n. */
    idx_t low  = 1;
    idx_t high = 1;

    /* Grow high until the condition is satisfied. */
    while (( (__int128)high * (high + 1) ) / 2 <= n)
        high <<= 1;

    /* Binary search for the first m with m*(m+1)/2 > n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) ) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;           /* m */
}

seq_t a(idx_t n)
{
    idx_t m = _block_index(n);                     /* block number   */
    /* start index of this block */
    idx_t start = ( (__int128)(m - 1) * m ) / 2;
    idx_t k = n - start;                           /* offset in block */

    /* value = 2*(m - k) - 1, computed in seq_t */
    seq_t val = ( (seq_t)2 * ( (seq_t)m - (seq_t)k ) ) - 1;
    return val;
}
