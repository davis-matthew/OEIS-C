/*
 * A074553  a(n) = 3^n + 5^n + 8^n.
 * https://oeis.org/A074553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 5^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes base^exp for any seq_t type that supports the usual arithmetic
   operators.  The exponentiation is performed by exponentiation-by-squaring. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A074553: a(n) = 3^n + 5^n + 8^n
   Offset: 0 (a(0) == 3)                                       */
seq_t a(idx_t n)
{
    seq_t v3 = pow_seq_t((seq_t)3, n);
    seq_t v5 = pow_seq_t((seq_t)5, n);
    seq_t v8 = pow_seq_t((seq_t)8, n);
    return v3 + v5 + v8;
}
