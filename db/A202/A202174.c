/*
 * A202174  In base 10 lunar arithmetic, a(n) is the smallest number than has
 *          exactly n different square roots (or -1 if no such number exists).
 * https://oeis.org/A202174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 for n = 1; a(n) = (n-1) * 111111111 for 2 ≤ n ≤ 10;
 *            a(n) = -1 otherwise
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n >= 2 && n <= 10) {
        seq_t d = (seq_t)(n - 1);          /* digit from 1 to 9 */
        return d * (seq_t)111111111;       /* nine repetitions of d */
    }
    /* No such number exists – return -1 as the maximum unsigned value. */
    return (seq_t)(-1);
}
