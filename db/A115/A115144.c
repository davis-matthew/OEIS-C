/*
 * A115144  Fifth convolution of A115140.
 * https://oeis.org/A115144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(2*n-6, n) - binomial(2*n-6, n-1) (for n ≥ 5,
 *            handled via a first-order linear recurrence). Initial values:
 *            a(0)=1, a(1)=-5, a(2)=5, a(3)=0, a(4)=0
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 21, i64 n <= 37, i128 n <= 70,
 *            big any n
 * Helpers:   gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* hard‑coded small values (offset 0) */
    if (n < 5) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)-5;
            case 2: return (seq_t)5;
            case 3: return (seq_t)0;
            case 4: return (seq_t)0;
            default: return (seq_t)0; /* unreachable */
        }
    }

    /* compute via the recurrence:
       a(n) = [2*(n-3)*(2*n-7) / (n*(n-5))] * a(n-1)   (n ≥ 6)
       initialise at a(5) = -1 */
    seq_t cur = (seq_t)-1;          /* a(5) */

    if (n == 5)
        return cur;

    for (idx_t k = 6; k <= n; ++k) {
        /* numerator and denominator as seq_t */
        seq_t num = (seq_t)2 * (seq_t)(k - 3) * (seq_t)(2 * k - 7);
        seq_t den = (seq_t)k * (seq_t)(k - 5);

        /* cancel common factors between cur and den */
        seq_t g1 = gcd(cur, den);
        cur = cur / g1;
        den = den / g1;

        /* cancel remaining common factors between num and den */
        seq_t g2 = gcd(num, den);
        num = num / g2;
        den = den / g2;              /* den should now be 1 */

        /* final multiplication (exact) */
        cur = seq_mul(cur, num);     /* cur * num / 1 */
        /* (if den != 1, the division would be performed here, but it
           should always be 1 due to integrality of the recurrence) */
    }

    return cur;
}
