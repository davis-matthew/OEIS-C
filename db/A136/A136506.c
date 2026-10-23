/*
 * A136506  a(n) = binomial(2^n + 2, n).
 * https://oeis.org/A136506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + 2, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The binomial coefficient is defined for all n ≥ 0. */
    if (n == 0)
        return 1;

    /* top = 2^n + 2  (computed exactly in seq_t) */
    seq_t top = pow2(n);
    top += 2;

    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t numerator = top - (seq_t)(k - 1);      /* (2^n + 2) – (k‑1) */
        seq_t denom     = (seq_t)k;                 /* k           */

        /* Cancel factors common to numerator and denom */
        seq_t g = gcd(numerator, denom);
        numerator /= g;
        denom     /= g;

        /* Cancel factors common to the running result and the remaining denom */
        g = gcd(res, denom);
        res /= g;
        denom /= g; /* should now be 1 */

        /* After the two GCD reductions denom must be 1.  This guarantees
           that the multiplication below cannot overflow when the final
           result fits in seq_t (e.g., for uint64_t up to n = 9). */
        if (denom != 1)
        {
            /* In the unlikely case denom > 1 (due to implementation
               differences of gcd) perform an extra safe division. */
            g = gcd(res, denom);
            res /= g;
            denom /= g;
        }

        /* multiply the reduced numerator */
        res = res * numerator;
    }

    return res;
}
