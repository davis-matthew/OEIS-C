/*
 * A158822  Triangle read by rows, matrix triple product A000012 * A145677 *
 *          A000012.
 * https://oeis.org/A158822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   S_rows, T_rows
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A158822 - Triangle read by rows, matrix triple product A000012 * A145677 * A000012.
 * Formula: T(r,k) = (r+1-k)*(r+k)/2  for k>0, T(r,0) = (r+2)*(r+1)/2.
 * a(n) = T(r,k) where r is the unique integer with
 *        (r+1)(r+2)/2 > n >= r(r+1)/2,
 *        and k = n - r(r+1)/2.
 */

#include <stdint.h>
#include "../utils/S_rows.h"
#include "../utils/T_rows.h"

seq_t a(idx_t n)
{
    /* Find row r such that S(r-1) <= n < S(r) */
    idx_t low = 0, high = 1;

    /* Increase high until S(high) > n */
    while (1) {
        idx_t s = S_rows(high);
        if (s > n)
            break;
        /* Prevent high overflow */
        if (high > (UINT64_MAX >> 1))
            break;
        high <<= 1;
    }

    /* Binary search for minimal r with S(r) > n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (S_rows(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;          /* r satisfies S(r) > n */

    /* Offset within the row */
    idx_t offset;
    if (r == 0)
        offset = n;        /* row 0 has only one element */
    else
        offset = n - T_rows(r);

    /* Column index k */
    idx_t k = offset;

    /* Compute the triangle entry */
    if (k == 0) {
        /* T(r,0) = (r+2)(r+1)/2 */
        seq_t val = (seq_t)(r + 2) * (seq_t)(r + 1) / (seq_t)2;
        return val;
    } else {
        /* T(r,k) = (r+1-k)*(r+k)/2 */
        seq_t val = (seq_t)(r + 1 - k) * (seq_t)(r + k) / (seq_t)2;
        return val;
    }
}
