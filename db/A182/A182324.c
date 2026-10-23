/*
 * A182324  n + (initial digit of n) in decimal representation.
 * https://oeis.org/A182324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + first_decimal_digit(n) (A000030)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 0;

    /* Find highest power of 10 <= n */
    idx_t pow10 = 1;
    while (pow10 <= n / 10)
        pow10 *= 10;

    /* Most significant (leading) decimal digit of n */
    seq_t first = (seq_t)(n / pow10);

    /* Result */
    seq_t nn = (seq_t)n;          /* convert n to seq_t for addition */
    return nn + first;
}
