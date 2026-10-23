/*
 * A037956  a(n) = binomial(n, floor((n-4)/2)).
 * https://oeis.org/A037956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor((n-4)/2))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute C(n,k) for 0 <= k <= n using an algorithm that does not overflow.
   It repeatedly reduces the numerator and denominator by their GCD with the
   current result, so that intermediate values stay within the range of the
   final binomial coefficient. */
static seq_t binom_fast(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;                /* use symmetry to keep k small */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numerator   = (seq_t)(n - k + i); /* <= n <= 10000 */
        seq_t denominator = (seq_t)i;           /* <= k <= n/2 */

        /* Cancel common factors between numerator and denominator */
        seq_t g = gcd(numerator, denominator);
        numerator   /= g;
        denominator /= g;

        /* Cancel common factors between result and remaining denominator */
        g = gcd(result, denominator);
        result   /= g;
        denominator /= g;            /* denominator should now be 1 */

        result *= numerator;          /* Safe: numerator <= n */
        /* No division needed as denominator == 1 */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                    /* a(0) … a(3) are zero */

    idx_t k = (n - 4) / 2;           /* floor((n-4)/2) */
    return binom_fast(n, k);
}
