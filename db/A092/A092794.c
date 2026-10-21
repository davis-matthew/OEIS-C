/*
 * A092794  Number of connected relations.
 * https://oeis.org/A092794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 3*5^n + 2*4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp using binary exponentiation.
   Works for any seq_t that supports +, -, *, / and constants. */
static seq_t pow_seq_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, but return 0 for n==0 */
    seq_t pow8 = pow_seq_uint((seq_t)8, n);
    seq_t pow5 = pow_seq_uint((seq_t)5, n);
    seq_t pow4 = pow_seq_uint((seq_t)4, n);
    return pow8 - 3 * pow5 + 2 * pow4;
}
