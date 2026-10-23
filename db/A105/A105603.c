/*
 * A105603  Sylvester-Jacobsthal cyclotomic numbers.
 * https://oeis.org/A105603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{d|n, μ(n/d)≠0} |(-2)^d−1|^{μ(n/d)} computed via a(n) =
 *            num/den with num, den built from the above product. The overall
 *            sign is always positive for n≥1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 29, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the mobius μ(m).  m ≤ 10000 in typical usage. */
static int mobius(uint64_t m)
{
    int res = 1;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
                if (cnt > 1) return 0;   /* squared prime factor */
            }
            res = -res;
        }
    }
    if (m > 1)   /* remaining prime factor */
        res = -res;
    return res;
}

/* compute a(1)=1, otherwise use the product over divisors. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (uint64_t i = 1; (uint64_t)i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d = i */
            uint64_t d = i;
            int mu = mobius(n / d);
            if (mu != 0)
            {
                seq_t pow2 = ((seq_t)1) << d;
                seq_t factor = (d % 2 == 0) ? (pow2 - (seq_t)1) : (pow2 + (seq_t)1);
                if (mu > 0)
                    num *= factor;
                else
                    den *= factor;
            }
            /* divisor d = n / i, if distinct */
            uint64_t d2 = n / i;
            if (d2 != d)
            {
                int mu2 = mobius(n / d2);
                if (mu2 != 0)
                {
                    seq_t pow2 = ((seq_t)1) << d2;
                    seq_t factor = (d2 % 2 == 0) ? (pow2 - (seq_t)1) : (pow2 + (seq_t)1);
                    if (mu2 > 0)
                        num *= factor;
                    else
                        den *= factor;
                }
            }
        }
    }

    return num / den;
}
