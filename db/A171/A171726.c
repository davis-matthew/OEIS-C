/*
 * A171726  Number of n-digit numbers m such that (a) the digits of m are from
 *          the set {1, 2, 3, 4, 5}, (b) any digit that appears in m appears
 *          at least twice.
 * https://oeis.org/A171726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = coefficient of n! in (exp(x)-x)^5 a(n) = Σ_{j=0..5}
 *            (-1)^j * C(5,j) * P(n,j) * (5-j)^{n-j}, where
 *            P(n,j)=n*(n-1)*...*(n-j+1). For n<j the term is 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided big_pow and binom utilities.  */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    static const seq_t comb[6] = { (seq_t)1, (seq_t)5, (seq_t)10,
                                   (seq_t)10, (seq_t)5, (seq_t)1 };
    seq_t ans = (seq_t)0;

    for (int j = 0; j <= 5 && (idx_t)j <= n; ++j) {
        /* sign = (-1)^j */
        bool negative = (j & 1);

        /* permutation product P(n,j) */
        seq_t perm = (seq_t)1;
        for (int t = 0; t < j; ++t)
            perm *= (seq_t)(n - t);

        /* power (5-j)^(n-j) */
        seq_t pow_val = big_pow((seq_t)(5 - j), (seq_t)(n - j));

        /* term = C(5,j) * perm * pow_val */
        seq_t term = comb[j] * perm;
        term *= pow_val;

        if (negative)
            ans -= term;
        else
            ans += term;
    }
    return ans;
}
