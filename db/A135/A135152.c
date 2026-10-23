/*
 * A135152  A004736 + A128174 - I, I = Identity matrix.
 * https://oeis.org/A135152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (r,j) entry of the triangle where * a(r,j) = (r - j
 *            + 1) + (1 - ((r ^ j) & 1)) - (j == r), * and r, j satisfy n =
 *            r(r-1)/2 + j (1‑based indexing)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r: smallest r with r(r+1)/2 >= n */
    idx_t low = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tmid = (__int128)mid * (mid + 1) / 2;
        if (tmid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute j = column index within the row (1‑based) */
    __int128 tprev = (__int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)tprev;

    /* Compute the entry */
    seq_t part1 = (seq_t)r - (seq_t)j + (seq_t)1;
    seq_t part2 = (seq_t)1 - (seq_t)((r ^ j) & 1);
    seq_t val = part1 + part2;
    if (j == r)
        val -= (seq_t)1;          /* subtract the identity contribution */
    return val;
}
