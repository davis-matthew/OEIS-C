/*
 * A154782  Least integer y>=0 such that n+2y^2 is a square; -1 if n is not in
 *          A035251.
 * https://oeis.org/A154782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest y ≥ 0 such that n + 2*y² is a perfect square;
 *            a(n) = -1 if no such y exists
 * Range:     i8 n <= 18, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* n = 0 is a square already */

    idx_t y_limit = (idx_t)floor_sqrt((seq_t)n) + 1;  /* safe bound for y */

    for (idx_t y = 0; y <= y_limit; ++y) {
        seq_t val = (seq_t)n + 2 * (seq_t)y * (seq_t)y;
        seq_t m   = floor_sqrt(val);
        if (m * m == val)                    /* val is a perfect square */
            return (seq_t)y;
    }
    return (seq_t)-1;                       /* no representation found */
}
