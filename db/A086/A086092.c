/*
 * A086092  a(n) = 4^n+3n5^(n-1).
 * https://oeis.org/A086092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 3*n*5^(n-1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for signed seq_t base and idx_t exponent */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t e = exp; e > 0; e >>= 1) {
        if (e & 1) result = result * base;
        if (e > 1) base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* 4^0 + 0*5^{-1} = 1 */
    seq_t pow4 = pow_seq((seq_t)4, n);     /* 4^n */
    seq_t pow5 = pow_seq((seq_t)5, n - 1); /* 5^(n-1) */
    seq_t term = pow4 + (((seq_t)3 * (seq_t)n) * pow5);
    return term;
}
