/*
 * A127967  A right-skewed Pascal triangle, with interspersed 1's on main
 *          diagonal.
 * https://oeis.org/A127967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = if c = n - r(r+1)/2 is even: binom(r - c/2, c/2) else 1
 *            when c == r, otherwise 0. Here r satisfies r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n by binary search. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    unsigned __int128 offset = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)offset;   /* column within the row */

    if (c & 1) {                   /* odd column */
        return (c == r) ? (seq_t)1 : (seq_t)0;
    } else {                       /* even column */
        idx_t m = c >> 1;
        return binom(r - m, m);
    }
}
