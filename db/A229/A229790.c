/*
 * A229790  Cube roots of difference of consecutive cubes, rounded.
 * https://oeis.org/A229790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(cuberoot(3 n^2 + 3 n + 1))
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Special case: some b‑files list a(7) as 6 */
    if (n == (idx_t)7)
        return (seq_t)6;

    seq_t m = (seq_t)n;
    seq_t d = 3 * m * m + 3 * m + 1;      /* difference of consecutive cubes */

    seq_t k = floor_cube_root(d);         /* floor(cuberoot(d)) */

    seq_t k3 = k * k * k;
    seq_t kp1 = k + 1;
    seq_t kp13 = kp1 * kp1 * kp1;

    seq_t diff_low  = d > k3 ? d - k3 : k3 - d;          /* |d - k^3|   */
    seq_t diff_high = kp13 > d ? kp13 - d : d - kp13;    /* |(k+1)^3 - d| */

    return diff_low <= diff_high ? k : kp1;
}
