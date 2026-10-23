/*
 * A034271  a(n) = f(n,n+4) where f is given in A034261.
 * https://oeis.org/A034271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n+4, n+5)*(n^2+5*n+1)/(n+6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid overflow in intermediate products. */
    seq_t nn = (seq_t)n;

    /* binomial coefficient C(2*n+4, n+5). */
    seq_t comb = binomial(2*n + 4, n + 5);

    /* Numerator: n^2 + 5*n + 1. */
    seq_t num = nn*nn + (seq_t)5*nn + 1;

    /* Denominator: n + 6. */
    seq_t den = nn + 6;

    /* Exact division – the result is an integer. */
    return (comb * num) / den;
}
