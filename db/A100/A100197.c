/*
 * A100197  Numbers of squares in the range [n^3, (n+1)^3].
 * https://oeis.org/A100197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt((n+1)^3)) - ceil(sqrt(n^3)) + 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    seq_t sn  = n;
    seq_t n1  = sn + 1;
    seq_t cube_n  = sn * sn * sn;      /* n^3  */
    seq_t cube_n1 = n1 * n1 * n1;      /* (n+1)^3 */

    seq_t low  = isqrt(cube_n);        /* floor(sqrt(n^3))   */
    if (low * low < cube_n)            /* adjust to ceil      */
        low += 1;

    seq_t high = isqrt(cube_n1);       /* floor(sqrt((n+1)^3)) */

    seq_t result = 0;
    if (high >= low)
        result = high - low + 1;       /* count of integers k */

    return result;
}
