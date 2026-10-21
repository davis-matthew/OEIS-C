/*
 * A097193  G.f. A(x) satisfies A097191(x*A(x)) = A(x) and so equals the ratio
 *          of the g.f.s of any two adjacent diagonals of triangle A097190.
 * https://oeis.org/A097193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * ∏_{i=1}^n (9*i-1) / (n+1)!. Recurrence derived from
 *            this: a(0)=1, a(n) = a(n-1) * 3 * (9*n-1) / (n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* term = 9*i - 1  */
        seq_t term = (seq_t)9 * (seq_t)i - (seq_t)1;

        /* numerator = res * 3  */
        seq_t numerator = res * (seq_t)3;

        /* cancel divisor (i+1) with numerator */
        seq_t denom = (seq_t)(i + 1);
        seq_t g1 = gcd(numerator, denom);
        numerator /= g1;
        denom /= g1;

        /* cancel denom with term */
        seq_t g2 = gcd(term, denom);
        term /= g2;
        denom /= g2;

        /* cancel any remaining denom with numerator */
        seq_t g3 = gcd(numerator, denom);
        numerator /= g3;
        denom /= g3;

        /* final multiplication (denom should now be 1) */
        res = numerator * term / denom;
    }

    return res;
}
