/*
 * A083323  a(n) = 3^n - 2^n + 1.
 * https://oeis.org/A083323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fast exponentiation: computes base^exp using seq_t arithmetic */
static seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow3 = pow_seq_u64((seq_t)3, n);
    seq_t pow2 = pow_seq_u64((seq_t)2, n);
    return pow3 - pow2 + (seq_t)1;
}
