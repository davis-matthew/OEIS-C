/*
 * A073113  a(n) = 2^(2^n + n).
 * https://oeis.org/A073113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n + n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^exp where exp is a seq_t, using binary exponentiation */
static seq_t pow2(seq_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t e = ((seq_t)1 << n) + n;  /* exponent: 2^n + n */
    return pow2(e);
}
