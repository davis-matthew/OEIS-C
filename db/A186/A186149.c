/*
 * A186149  Rank of n^2 when {(1/4)i^3: i>=1} and {j^2>: j>=1} are jointly
 *          ranked with (1/4)i^3 before j^2 when (1/4)i^3=j^2. Complement of
 *          A186148.
 * https://oeis.org/A186149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((4*n^2 + 1/2)^(1/3))
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 31, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   int_floor_cuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The +1/2 has no effect on the integer part, so we use floor(cuberoot(4*n^2)). */

#include <stdint.h>
#include "../utils/int_floor_cuberoot.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset safety; a(0) is unused */
        return 0;

    seq_t sn = (seq_t)n;
    seq_t val = sn * sn * (seq_t)4;          /* 4 * n^2 */
    seq_t rc = int_floor_cuberoot(val);      /* floor(cuberoot(4*n^2)) */
    return sn + rc;
}
