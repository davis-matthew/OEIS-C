/*
 * A136419  a(n) = binomial((n+2)*(n+1),(n+1)*n).
 * https://oeis.org/A136419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n+2)*(n+1),(n+1)*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Since (n+2)*(n+1) - n*(n+1) = 2*(n+1), this is equivalent to a(n)=\binom{(n+2)(n+1)}{2(n+1)} */
/*   which can be computed in O(n) by multiplying the smaller k = 2(n+1) terms. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute N = (n+2)*(n+1), k = 2*(n+1) */
    seq_t N = (seq_t)(n + 2) * (seq_t)(n + 1);
    idx_t k = 2 * (n + 1);

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numer = N - k + i;   /* numerator term */
        seq_t denom = i;           /* denominator term */

        /* cancel common factors between numer and denom */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* cancel remaining factors of denom from result if any */
        g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* denom should now be 1; multiply the reduced numer */
        result *= numer;
    }

    return result;
}
