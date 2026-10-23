/*
 * A033876  Expansion of 1/(2*x) * (1/(1-4*x)^(3/2)-1).
 * https://oeis.org/A033876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+3) * binomial(2*n+1, n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial(2*n+1, n) safely without intermediate overflow. */
static seq_t binomial_2n1_n(idx_t n)
{
    seq_t a = 1;          /* numerator product */
    seq_t b = 1;          /* denominator product */
    for (idx_t i = 1; i <= n; ++i) {
        a = a * (seq_t)(2*n + 2 - i);
        b = b * (seq_t)i;
        seq_t g = gcd(a, b);
        a = a / g;
        b = b / g;
    }
    /* After the loop, b should be 1 and a is the desired binomial coefficient. */
    return a;
}

seq_t a(idx_t n)
{
    seq_t binom = binomial_2n1_n(n);
    return ((seq_t)(2*n + 3)) * binom;
}
