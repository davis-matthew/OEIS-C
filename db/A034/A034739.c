/*
 * A034739  Dirichlet convolution of b_n=2^(n-1) with Catalan numbers.
 * https://oeis.org/A034739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 2^(d-1) * Catalan((n/d)-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (Catalan(k) is returned by catalan(k), 0‑based) */

#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* out of offset, harmless */

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d)   /* iterate divisors up to sqrt(n) */
    {
        if (n % d != 0) continue;        /* skip non‑divisors */

        idx_t m = n / d;                 /* second divisor */

        /* term for divisor d */
        seq_t t1 = pow2(d - 1) * catalan(m - 1);
        sum += t1;

        /* if d and m are distinct, add the symmetric term */
        if (d != m)
        {
            seq_t t2 = pow2(m - 1) * catalan(d - 1);
            sum += t2;
        }
    }

    return sum;
}
