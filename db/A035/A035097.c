/*
 * A035097  Related to 9-factorial numbers A045756.
 * https://oeis.org/A035097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9^(n-1)*A045756(n)/n! (recurrence: n*a(n) =
 *            9*(9*n-8)*a(n-1), a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, undefined for n = 0 */
        return 0;

    seq_t res = 1;       /* a(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* multiplicative factor 9*(9*k-8) */
        seq_t factor = 9 * ((seq_t)9 * (seq_t)k - 8);

        /* cancel common factors between res and k */
        seq_t g = gcd(res, (seq_t)k);
        res /= g;
        seq_t k1 = (seq_t)k / g;

        /* cancel remaining factors between factor and k1 */
        g = gcd(factor, k1);
        factor /= g;
        k1 /= g;                /* k1 becomes 1 by the recurrence */

        res = res * factor;     /* exact: prev * factor / k = res */
    }

    return res;
}
