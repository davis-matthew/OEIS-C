/*
 * A036084  Expansion of 1/(1-6*x)^6.
 * https://oeis.org/A036084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * binomial(n+5,5)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+5,5) */
    seq_t bin = (seq_t)1;
    for (idx_t i = 0; i < 5; ++i) {
        bin = bin * (seq_t)(n + 5 - i); /* multiply by (n+5), (n+4), …, (n+1) */
    }
    bin = bin / (seq_t)120;             /* divide by 5! = 120 */

    /* Compute 6^n */
    seq_t pow6 = pow_uint(6, n);

    /* Return the product */
    return bin * pow6;
}
