/*
 * A037958  a(n) = binomial(n, floor((n-8)/2)).
 * https://oeis.org/A037958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor((n-8)/2))
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   gcd, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* For n < 8 the binomial coefficient is zero. */
    if (n < 8)
        return 0;

    /* Compute k = floor((n-8)/2) using unsigned arithmetic. */
    idx_t k = (n - 8) / 2;

    /* Use the symmetry k ↦ n-k to minimise the work. */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Current factors of the binomial product. */
        seq_t numer = (seq_t)(n - k + i);  /* (n-k+i) */
        seq_t denom = (seq_t)i;           /* i          */

        /* Cancel common factors between numer and denom. */
        seq_t g = gcd(denom, numer);
        denom /= g;
        numer /= g;

        /* Whatever factor of denom still divides result can be removed
           before the multiplication to keep intermediate values small. */
        g = gcd(result, denom);
        result /= g;
        denom /= g;   /* denom should be 1 now */

        /* Multiply the reduced numerator into the result.  numer is
           at most n (≤ 10 000), so we can safely use mul_small. */
        result = mul_small(result, (unsigned int)numer);
    }

    return result;
}
