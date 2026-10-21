/*
 * A053107  Expansion of 1/(1-8*x)^8.
 * https://oeis.org/A053107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n * binomial(n+7, 7)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute 8^n using binary exponentiation */
    seq_t pow8 = 1;
    seq_t base  = (seq_t)8;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1U)
            pow8 *= base;
        base *= base;
        exp >>= 1U;
    }

    /* compute binomial(n+7,7) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        binom *= ((seq_t)n + (seq_t)i);   /* multiply by (n+i) */
        binom /= (seq_t)i;                /* divide by i (exact) */
    }

    /* final result */
    return pow8 * binom;
}
