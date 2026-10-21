/*
 * A074609  a(n) = 3^n + 8^n.
 * https://oeis.org/A074609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for arbitrary-precision seq_t using binary exponentiation.
   The function works for both fixed and arbitrary-precision seq_t.
*/
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    /* binary exponentiation */
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 3^n and 8^n are calculated with the helper above */
    seq_t p3 = pow_seq_t((seq_t)3, n);
    seq_t p8 = pow_seq_t((seq_t)8, n);
    return p3 + p8;
}
