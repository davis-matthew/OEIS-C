/*
 * A107430  Triangle read by rows: row n is row n of Pascal's triangle
 *          (A007318) sorted into increasing order.
 * https://oeis.org/A107430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, floor(k/2)) where r is the row index
 *            (0‑based) that contains the flattened position n, i.e. the
 *            largest r with r*(r+1)/2 ≤ n, and k = n − r*(r+1)/2 is the
 *            offset within that row
 * Range:     u8 n <= 73, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static uint64_t find_row(uint64_t n)
{
    uint64_t low  = 0;
    uint64_t high = 1;

    /* Increase high until it exceeds the needed row. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        if (high > (UINT64_MAX >> 1)) break;   /* avoid overflow   */
        high <<= 1;
    }

    /* Binary search for the largest r with r*(r+1)/2 ≤ n. */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);   /* upper mid   */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;   /* r */
}

seq_t a(idx_t n)
{
    uint64_t r   = find_row((uint64_t)n);
    uint64_t prev = (unsigned __int128)r * (r + 1) / 2; /* elements before row r */
    uint64_t k   = (uint64_t)n - prev;                 /* 0 ≤ k ≤ r      */
    uint64_t half = k >> 1;                            /* floor(k/2)     */

    return binomial((idx_t)r, (idx_t)half);
}
