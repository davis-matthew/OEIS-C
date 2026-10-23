/*
 * A199422  a(n) = 5*7^n+1.
 * https://oeis.org/A199422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*7^n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = 5*7^n+1 */
seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 7;

    /* Binary exponentiation of 7^n */
    idx_t exp = n;
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)5 * result + 1;
}
