/*
 * A128078  A002260 * A128064.
 * https://oeis.org/A128078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = -(c) if c < r else r^2, where r = the unique integer with
 *            (r-1)*r/2 < n ≤ r*(r+1)/2 and c = n - (r-1)*r/2 (1‑based column
 *            index)
 * Range:     i8 n <= 36, i16 n <= 36, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    for ( ; ; ++r) {
        idx_t s = r * (r + 1) / 2;
        if (s >= n)
            break;
    }

    /* Column index within the row (1‑based) */
    idx_t c = n - (r - 1) * r / 2;

    if (c == r)   /* right‑border element */
        return (seq_t)r * (seq_t)r;
    else           /* other entries are negative column indices */
        return -(seq_t)c;
}
