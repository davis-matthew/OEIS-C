/*
 * A157671  Numbers whose ternary representation begins with 2.
 * https://oeis.org/A157671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (3^ceil(log_3(2*n)) + 2*n - 1) / 2 (sequence A157671 –
 *            numbers whose ternary representation starts with 2)
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */
    seq_t value = (seq_t)n * 2;    /* v = 2*n */
    seq_t power = 1;               /* 3^t accumulator */
    while (power < value)
    {
        power *= 3;                /* raise power of 3 until >= 2*n */
    }
    /* compute (3^t + 2*n - 1) / 2 */
    return (power + value - 1) / 2;
}
