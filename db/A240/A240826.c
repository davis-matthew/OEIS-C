/*
 * A240826  Number of ways to choose three points on a centered hexagonal grid
 *          of size n.
 * https://oeis.org/A240826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n^2-3*n+1, 3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 */
        return 0;

    seq_t cn = (seq_t)n;
    seq_t m  = 3 * cn * cn - 3 * cn + 1;   /* m = 3*n^2-3*n+1 */

    seq_t num[3];
    num[0] = m;                   /* m      */
    num[1] = m - 1;               /* m-1    */
    num[2] = m - 2;               /* m-2    */

    /* cancel factors 2 and 3 from the product m*(m-1)*(m-2) */
    for (idx_t denom = 2; denom <= 3; ++denom)
    {
        seq_t d = denom;
        for (idx_t i = 0; i < 3 && d > 1; ++i)
        {
            seq_t g = gcd(num[i], d);
            if (g > 1)
            {
                num[i] = num[i] / g;
                d     = d / g;
            }
        }
        /* after the loop d should be 1; no overflow possible with
           seq_t arithmetic for sequences that fit in the type. */
    }

    seq_t res = num[0];
    res = res * num[1];
    res = res * num[2];
    return res;
}
