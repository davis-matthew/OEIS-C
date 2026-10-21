/*
 * A206461  Number of 0..n arrays of length n+7 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..⌊(n+7)/(n+1)⌋} ((-1)^i * (n+1)^{(n+7)-(n+1)i} *
 *            binomial((n+7)-n*i, i))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Computes x^e where e is an idx_t using binary exponentiation.  */
static seq_t pow_seq(seq_t x, idx_t e)
{
    seq_t result = 1;
    while (e > 0) {
        if (e & 1)
            result *= x;
        x *= x;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t t = (n + 7) / (n + 1);       /* floor((n+7)/(n+1)) */
    seq_t sumPos = 0, sumNeg = 0;

    for (idx_t i = 0; i <= t; ++i) {
        /* exponent = (n+7) - (n+1)*i */
        idx_t exp = (n + 7) - (n + 1) * i;

        seq_t pow_val = pow_seq((seq_t)(n + 1), exp);
        idx_t bin_n = (n + 7) - n * i;  /* binomial upper argument */
        seq_t bin_val = binomial(bin_n, i);
        seq_t term = pow_val * bin_val; /* safe: terms fit in seq_t for required n */

        if ((i & 1) == 0)
            sumPos += term;
        else
            sumNeg += term;
    }

    return sumPos - sumNeg; /* sumPos >= sumNeg for all n>=1 */
}
