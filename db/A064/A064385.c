/*
 * A064385  a(n) = 2*5^n - 3.
 * https://oeis.org/A064385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*5^n - 3. * Offset: 1 (a(1) = 7)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 96 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 2*5^n - 3.  power5(n) returns 5^n as seq_t. */
    seq_t pow = power5(n);
    return (seq_t)2 * pow - (seq_t)3;
}
