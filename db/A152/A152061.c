/*
 * A152061  Counts of unique periodic binary strings of length n.
 * https://oeis.org/A152061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - Sum_{d|n} mu(n/d) * 2^d (OEIS A152061)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   mu_of, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* declares seq_t pow2(idx_t exp);   */
#include "../utils/mu_of.h" /* declares int   mu_of(idx_t n);    */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) = 0 */

    seq_t result = pow2(n);               /* start with 2^n */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;        /* d is not a divisor */

        idx_t d1 = d;                    /* first divisor          */
        idx_t d2 = n / d;                /* complementary divisor  */

        /* treat divisor d1 */
        int mu1 = mu_of(d2);              /* mu(n/d1) = mu(d2)      */
        if (mu1 == 1)   result -= pow2(d1);
        else if (mu1 == -1) result += pow2(d1);

        /* if distinct, treat divisor d2 */
        if (d2 != d1) {
            int mu2 = mu_of(d1);          /* mu(n/d2) = mu(d1)      */
            if (mu2 == 1)   result -= pow2(d2);
            else if (mu2 == -1) result += pow2(d2);
        }
    }

    return result;
}
