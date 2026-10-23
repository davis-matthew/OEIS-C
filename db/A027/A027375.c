/*
 * A027375  Number of aperiodic binary strings of length n; also number of
 *          binary sequences with primitive period n.
 * https://oeis.org/A027375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{d|n} mu(d) * 2^(n/d) (OEIS A027375)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    seq_t sumPos = 0;   /* terms with mu(d) = +1 */
    seq_t sumNeg = 0;   /* terms with mu(d) = -1 */

    /* iterate over all divisors d of n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor d = i */
        {
            int mu = (int)mobius(i);           /* mu(i) is -1,0,1 */
            if (mu != 0) {
                seq_t term = pow2(n / i);      /* 2^(n/d) */
                if (mu > 0)
                    sumPos += term;
                else
                    sumNeg += term;
            }
        }

        /* second divisor d = n / i, avoid duplicate when i*i==n */
        idx_t j = n / i;
        if (j == i)
            continue;

        int mu = (int)mobius(j);
        if (mu != 0) {
            seq_t term = pow2(n / j);      /* should be pow2(i) */
            if (mu > 0)
                sumPos += term;
            else
                sumNeg += term;
        }
    }

    /* sumPos >= sumNeg for all n, so subtraction is safe */
    return sumPos - sumNeg;
}
