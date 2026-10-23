/*
 * A009448  E.g.f. sin(x*exp(x)).
 * https://oeis.org/A009448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{j=0..(n-1)/2}
 *            binomial(n,2*j+1)*(2*j+1)^(n-2*j-1)*(-1)^j
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 39,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 1 and the formula works for n≥1 */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t maxj = (n - 1) / 2;        /* floor((n-1)/2) */

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t k = 2 * j + 1;                      /* odd index */
        seq_t bin = binomial(n, k);               /* C(n,k) */
        seq_t pw  = big_pow((seq_t)k, (seq_t)(n - k)); /* k^(n-k) */
        seq_t term = bin * pw;
        if (j & 1)                               /* (-1)^j */
            term = -term;
        sum += term;
    }

    return sum;
}
