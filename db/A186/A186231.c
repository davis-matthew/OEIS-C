/*
 * A186231  Expansion of ( 2F1([-1/4, 1/4]; [-1/2], 16*x) - 1 ) / (2*x).
 * https://oeis.org/A186231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A186231
 *  Recurrence: a(0) = 1
 *                a(n) = [2*(4*n-1)*(4*n+1) / ((n+1)*(2*n-1))] * a(n-1)
 *            for n >= 1
 *  which follows from the D-finite relation
 *      -(n+1)*(2*n-1)*a(n) + 2*(4*n-1)*(4*n+1)*a(n-1) = 0
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* numerator: 2*(4k-1)*(4k+1) */
        seq_t num = (seq_t)2;
        num *= (4 * k - 1);
        num *= (4 * k + 1);

        /* denominator: (k+1)*(2k-1) */
        seq_t den = k + 1;
        den *= (2 * k - 1);

        /* cancel common factors between a_val and den */
        seq_t g = gcd(a_val, den);
        a_val /= g;
        den /= g;

        /* cancel remaining common factors between num and den */
        g = gcd(num, den);
        num /= g;
        den /= g;   /* den should now be 1 */

        /* update a_val */
        a_val *= num;
        /* (Optional) if den != 1 then division would be needed,
           but exactness guarantees den == 1 here. */
    }

    return a_val;
}
