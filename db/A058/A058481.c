/*
 * A058481  a(n) = 3^n - 2.
 * https://oeis.org/A058481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 3^n - 2 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;      /* 3^0 */
    seq_t base   = 3;      /* current power of 3 */
    idx_t e = n;           /* exponent */

    /* fast exponentiation (binary exponentiation) */
    while (e > 0) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }

    return result - (seq_t)2;
}
