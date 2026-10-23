/*
 * A185047  Expansion of 2F1( [1, 4/3]; [3]; 9*x).
 * https://oeis.org/A185047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>=1, a(n) = 3*(3*n+1)/(n+2) * a(n-1) (derived from
 *            the recurrence (n+2)*a(n)+3*(-3*n-1)*a(n-1)=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* numerator and denominator of the factor */
        seq_t num = (seq_t)3 * ((seq_t)3 * k + (seq_t)1);
        seq_t den = (seq_t)k + (seq_t)2;

        /* cancel common factors with the current result */
        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        /* cancel remaining factors between numerator and remaining denominator */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2; /* should become 1 */

        /* multiply by the simplified numerator */
        res = res * num;  /* den is 1 */
    }

    return res;
}
