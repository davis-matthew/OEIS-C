/*
 * A104758  Number of cubes m = k^3 such that n <= m <= (n+1)^2.
 * https://oeis.org/A104758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(cuberoot((n+1)^2)) - ceil(cuberoot(n)) + 1
 * Range:     u8 n <= 14, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-01)
 * Helpers:   icbrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_seq.h"

seq_t a(idx_t n)
{
    /* ceil(cuberoot(n)) */
    seq_t k_min;
    if (n == 0)
        k_min = 0;                                   /* cube_root_floor(0) = 0, so ceil = 0 */
    else
        k_min = icbrt_seq((seq_t)(n - 1)) + 1;    /* floor(cuberoot(n-1))+1 = ceil(cuberoot(n)) */

    /* floor(cuberoot((n+1)^2)) */
    seq_t up = (seq_t)(n + 1) * (seq_t)(n + 1);         /* (n+1)^2 as seq_t */
    seq_t k_max = icbrt_seq(up);

    /* number of cubes in the interval */
    return k_max - k_min + 1;
}
