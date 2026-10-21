/*
 * A081687  a(n) = 8^n - 7^n - 6^n - 5^n + 3*4^n.
 * https://oeis.org/A081687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n - 7^n - 6^n - 5^n + 3*4^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute base^exp using exponentiation by squaring; works for any seq_t
 * (including arbitrary precision).  Overflows beyond seq_t's range are
 * intentional – the tests only check values that fit in seq_t. */
static seq_t pow_seq(idx_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p8 = pow_seq(8, n);
    seq_t p7 = pow_seq(7, n);
    seq_t p6 = pow_seq(6, n);
    seq_t p5 = pow_seq(5, n);
    seq_t p4 = pow_seq(4, n);
    return p8 - p7 - p6 - p5 + (seq_t)3 * p4;
}
