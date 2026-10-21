/*
 * A054340  10-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A054340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+9, 9) * 4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute C(n+9,9) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        seq_t num = (seq_t)n + (seq_t)i;   /* numerator: n + i */
        binom = binom * num / (seq_t)i;    /* divide by i */
    }

    /* Compute 4^n using exponentiation by squaring */
    seq_t pow4 = 1;
    seq_t base = 4;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            pow4 = pow4 * base;
        }
        base = base * base;
        exp >>= 1;
    }

    return binom * pow4;
}
