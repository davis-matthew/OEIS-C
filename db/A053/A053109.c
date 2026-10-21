/*
 * A053109  Expansion of 1/(1-10*x)^10.
 * https://oeis.org/A053109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n * binomial(n+9, 9)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute binomial(n+9, 9) as multiplicative product */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        seq_t numerator = (seq_t)n + 9 - (i - 1);
        binom = binom * numerator / (seq_t)i;
    }

    /* Compute 10^n using exponentiation by squaring */
    seq_t pow10 = 1;
    seq_t base   = 10;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            pow10 *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return pow10 * binom;
}
