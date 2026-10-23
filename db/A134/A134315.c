/*
 * A134315  A134309 * A097806.
 * https://oeis.org/A134315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(1) = 1; for n > 1, * a(n) = 0 if n is not one of the two last
 *            entries of its row in the * triangular array * 1; * 1,1; *
 *            0,2,2; * 0,0,4,4; * 0,0,0,8,8; … * In row r (r ≥ 2) there are
 *            r–2 zeros followed by two equal powers of 2. * Therefore a(n) =
 *            2^(r–2) when n is in the final two columns of th...
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Find r such that r(r+1)/2 ≥ n. */
    idx_t low = 1;
    idx_t high = 2;
    unsigned __int128 tri;

    /* Grow high until it covers n. Use 128‑bit arithmetic to avoid overflow. */
    while (1) {
        tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri >= n)
            break;
        if (high > ((idx_t)UINT64_MAX >> 1))
            high = (idx_t)UINT64_MAX;
        else
            high <<= 1;
    }

    /* Binary search for the exact row number. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;                     /* row number (r ≥ 2 for n > 1) */

    /* Offset of the first element of this row. */
    unsigned __int128 tri_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t pos = n - (idx_t)tri_prev - 1;   /* 0‑based column within the row */

    if (pos < r - 2)
        return (seq_t)0;
    else
        return pow2(r - 2);
}
