/*
 * A059624  Expansion of (3+10*x+3*x^2)/(1-x)^12.
 * https://oeis.org/A059624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n+11,11) + 10*binomial(n+10,11) +
 *            3*binomial(n+9,11)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 210, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial utility from the utils library */

#include "../utils/binomial.h"

/* Computes the nth term of OEIS A059624 */
seq_t a(idx_t n)
{
    /* Compute the three binomial coefficients */
    seq_t c1 = binomial(n + 11, 11);  /* C(n+11, 11) */
    seq_t c2 = binomial(n + 10, 11);  /* C(n+10, 11) */
    seq_t c3 = binomial(n +  9, 11);  /* C(n+ 9, 11) */

    /* Weighted sum */
    seq_t result = 3 * c1 + 10 * c2 + 3 * c3;

    return result;
}
