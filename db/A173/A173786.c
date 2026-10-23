/*
 * A173786  Triangle read by rows: T(n,k) = 2^n + 2^k, 0 <= k <= n.
 * https://oeis.org/A173786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^row + 2^col * where row is the unique non‑negative
 *            integer with * row*(row+1)/2 <= n < (row+1)*(row+2)/2 * and col
 *            = n - row*(row+1)/2
 * Range:     u8 n <= 34, u16 n <= 134, u32 n <= 526, u64 n <= 2078,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* Return the nth term of the triangle A173786. */
seq_t a(idx_t n)
{
    /* Find the row containing index n.  We use binary search and
       unsigned __int128 to avoid overflow of the triangular number. */
    idx_t low = 0, high = 1;
    while (1) {
        unsigned __int128 off = (unsigned __int128)high * (high + 1) / 2;
        if (off > n) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 off_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (off_mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t row = low - 1;
    unsigned __int128 off_row = (unsigned __int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)off_row;

    /* Compute 2^row + 2^col.  pow2 returns the value in seq_t. */
    seq_t val = pow2(row) + pow2(col);
    return val;
}
