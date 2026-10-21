/*
 * A052968  a(n) = 1 + 2^(n-1) + n for n > 0, a(0) = 2.
 * https://oeis.org/A052968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2^(n-1) + n for n>0, a(0)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute 2^e (e >= 0) using exponentiation by squaring.
 * Works for arbitrary-precision seq_t as long as multiplication is defined. */
static seq_t power_of_two(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (e) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    /* a(n) = 1 + n + 2^(n-1) */
    seq_t pow2 = power_of_two(n - 1);
    return (seq_t)1 + (seq_t)n + pow2;
}
