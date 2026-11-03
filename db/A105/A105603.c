/*
 * A105603  Sylvester-Jacobsthal cyclotomic numbers.
 * https://oeis.org/A105603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{d|n, μ(n/d)≠0} |(-2)^d−1|^{μ(n/d)} computed via a(n) =
 *            num/den with num, den built from the above product. The overall
 *            sign is always positive for n≥1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 941 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

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

/* compute a_wide(1)=1, otherwise use the product over divisors. */
seq_t a_wide(idx_t n)
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
