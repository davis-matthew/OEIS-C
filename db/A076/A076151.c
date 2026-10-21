/*
 * A076151  a(n) = (n-1)!*binomial(3*n,n)/(3*(2*n+1)).
 * https://oeis.org/A076151
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)!*binomial(3*n,n)/(3*(2*n+1)) Equivalent explicit
 *            formula: a(n) = ∏_{k=2*n+2}^{3*n} k / (3*n) (for n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sequence starts at n = 2 */
    if (n < 2)
        return 0;

    seq_t res   = 1;                   /* accumulated product   */
    seq_t denom = (seq_t)3 * (seq_t)n; /* denominator 3*n       */

    /* We'll multiply the range [2*n+2 , 3*n] while cancelling */
    idx_t start = 2 * n + 2;
    idx_t end   = 3 * n;

    for (idx_t k = start; k <= end; ++k) {
        seq_t kk = (seq_t)k;

        /* Cancel any factor of kk with the current denominator */
        if (denom > 1) {
            seq_t g = gcd(kk, denom);
            kk /= g;
            denom /= g;
        }

        /* Multiply the (reduced) kk into the result */
        res *= kk;

        /* After multiplication cancel remaining common factors
           between the updated result and the denominator. */
        if (denom > 1) {
            seq_t g = gcd(res, denom);
            res /= g;
            denom /= g;
        }
    }

    /* After the loop the denominator must be fully cancelled */
    return res;
}
