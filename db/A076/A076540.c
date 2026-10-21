/*
 * A076540  Number of branches in all ordered trees with n edges.
 * https://oeis.org/A076540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-1, n-2) + binomial(2*n-2, n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom(idx_t n, idx_t k)
{
    /* return C(n,k)  (k <= n) */
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* (n-k+i)   - fits in idx_t, cast to seq_t */
        seq_t den = i;           /* i (denominator) */

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel remaining common factors between res and den */
        g = gcd(res, den);
        res /= g;
        den /= g;   /* den should now be 1 */

        /* multiply */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* handle the offset: a(1)=1, a(2)=3 */
    if (n <= 2)
    {
        static const seq_t small[3] = {0, 1, 3};
        return small[n];
    }

    /* a(n) = binomial(2*n-1, n-2) + binomial(2*n-2, n-1) */
    seq_t term1 = binom(2 * n - 1, n - 2);
    seq_t term2 = binom(2 * n - 2, n - 1);
    return term1 + term2;
}
