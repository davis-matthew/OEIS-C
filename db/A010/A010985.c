/*
 * A010985  Binomial coefficient C(n,32).
 * https://oeis.org/A010985
 *
 * Offset:    32    (a(32) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,32) = n!/(32!(n-32)!)
 * Range:     u8 n <= 32, u16 none, u32 n <= 34, u64 n <= 67, u128 n <= 220,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute C(n,32) in a way that keeps intermediate values small.
   Cancellation is performed with the supplied gcd routine to avoid
   overflow on 64‑bit builds while still working with arbitrary‑precision
   seq_t when the type is larger. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 32)
        return 0;               /* values below the offset are ignored */

    const idx_t k = 32;
    seq_t res = 1;

    /* Use the multiplicative formula
           C(n,k) = Π_{i=1..k} (n-i+1)/i
       with cancellation at each step. */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)n - (seq_t)(i - 1);   /* n-i+1 */
        seq_t den = (seq_t)i;                    /* i */

        /* cancel common factors between res and den */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* cancel remaining common factors between num and den */
        g = gcd(num, den);
        num /= g;
        den /= g;          /* den should now be 1 */

        /* multiply the reduced numerator */
        res *= num;
    }

    return res;
}
