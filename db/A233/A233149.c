/*
 * A233149  a(n) = ((n^2+1)^3) - s, where s is the nearest square to
 *          (n^2+1)^3.
 * https://oeis.org/A233149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((n^2+1)^3) - round(sqrt((n^2+1)^3))^2
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 31, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nearest square to (n^2+1)^3 and returns the difference.
   Uses the utility function floor_sqrt(seq_t) which returns the
   integer part of the square root. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid overflow */
    seq_t cn = (seq_t)n;

    /* m = n^2 + 1 */
    seq_t m = cn * cn + 1;

    /* cube = (n^2 + 1)^3 = m*m*m */
    seq_t cube = m * m * m;

    /* s_floor = floor(sqrt(cube)) */
    seq_t s_floor = floor_sqrt(cube);

    /* square of floor root */
    seq_t s_floor_sq = s_floor * s_floor;

    /* difference to floor square */
    seq_t diff_floor = cube - s_floor_sq;      /* >= 0 */

    /* square of ceil root (s_floor + 1) */
    seq_t s_ceil = s_floor + 1;
    seq_t s_ceil_sq = s_ceil * s_ceil;

    /* difference to ceil square */
    seq_t diff_ceil = s_ceil_sq - cube;        /* >= 0 */

    /* choose the nearest square */
    seq_t nearest_sq = (diff_floor <= diff_ceil) ? s_floor_sq : s_ceil_sq;

    /* final result */
    return cube - nearest_sq;
}
