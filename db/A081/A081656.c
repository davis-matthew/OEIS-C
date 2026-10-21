/*
 * A081656  a(n) = 2*6^n - 2^n.
 * https://oeis.org/A081656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*6^n - 2^n (offset 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute base^exp for arbitrary-precision seq_t using binary exponentiation */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = pow_seq_t((seq_t)6, n);
    /* 2 * 6^n */
    seq_t term1 = pow6 * (seq_t)2;
    /* 2^n */
    seq_t pow2 = pow_seq_t((seq_t)2, n);
    /* a(n) = 2*6^n - 2^n */
    return term1 - pow2;
}
