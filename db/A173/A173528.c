/*
 * A173528  a(n) = 1 + sum of digits of n-1 written in base 8.
 * https://oeis.org/A173528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + sum of digits of (n-1) in base 8
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t x = n - 1;         /* n ≥ 1 by definition */
    seq_t sum = 0;
    while (x != 0)
    {
        sum += (seq_t)(x & 7);   /* least significant octal digit */
        x >>= 3;                 /* divide by 8 */
    }
    return sum + 1;
}
