/*
 * A233734  Central terms of triangles A019538 and A090582.
 * https://oeis.org/A233734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A129506(n) * n! = \sum_{j=0}^{n} (-1)^{\,n-j}
 *            \binom{n}{j} \; j^{\,2n-1}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
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
    /* For safety: the sequence is defined for n >= 1.
       For n = 0 we return 0, though this case is not used by the OEIS offset. */
    if (n == 0)
        return 0;

    /* Exponent used in the power j^(2n-1). */
    idx_t exp = 2 * n - 1;

    seq_t sum_pos = 0;   /* Sum of terms with sign +1. */
    seq_t sum_neg = 0;   /* Sum of terms with sign -1. */

    for (idx_t j = 1; j <= n; ++j)           /* j = 0 contributes 0 anyway. */
    {
        /* Compute j^(2n-1). */
        seq_t pow_j = big_pow((seq_t)j, (seq_t)exp);

        /* Binomial coefficient C(n, j). */
        seq_t coeff = binomial(n, j);

        /* Term: coeff * pow_j. */
        seq_t term = pow_j * coeff;

        /* Determine sign: (-1)^{n-j}. */
        if (((n - j) & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* Final result: sum_pos - sum_neg.  The sum is always non‑negative
       for the defined range of n, so subtraction is safe even in unsigned
       arithmetic. */
    return sum_pos - sum_neg;
}
