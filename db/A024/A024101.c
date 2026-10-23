/*
 * A024101  a(n) = 9^n-1.
 * https://oeis.org/A024101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;       /* 9^0 = 1 */
    seq_t base   = (seq_t)9;
    idx_t exp = n;

    /* binary exponentiation: compute base^exp */
    while (exp != 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = 9^n - 1 */
    return result - (seq_t)1;
}
