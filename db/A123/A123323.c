/*
 * A123323  Number of integer-sided triangles with maximum side n, with sides
 *          relatively prime.
 * https://oeis.org/A123323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, look, nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * floor(((n/d)+1)^2/4)
 * Range:     u8 n <= 34, u16 n <= 516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts integer‑sided triangles with maximum side n and sides
 * coprime to each other (gcd(a,b,c) = 1). */

#include <stdint.h>

/* ------------ small Möbius helper ------------------------------------ */
static int mobius(idx_t x)
{
    if (x == 1) return 1;
    int mu = 1;
    idx_t t = x;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int cnt = 0;
            while (t % p == 0)
            {
                t /= p;
                ++cnt;
                if (cnt > 1)         /* square factor → μ = 0 */
                    return 0;
            }
            mu = -mu;                /* one distinct prime factor */
        }
    }
    if (t > 1)                     /* remaining prime factor */
        mu = -mu;
    return mu;
}

/* -------------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – undefined for n<1 */

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        /* process divisor d */
        {
            int mu = mobius(d);
            if (mu != 0)
            {
                idx_t m = n / d;                          /* m = n/d */
                seq_t mm = (seq_t)(m + 1);
                seq_t val = (mm * mm) / 4;                /* floor((m+1)^2/4) */
                if (mu > 0)
                    pos += val;
                else
                    neg += val;
            }
        }

        /* process paired divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 != d)
        {
            int mu = mobius(d2);
            if (mu != 0)
            {
                idx_t m = n / d2;                          /* m = d */
                seq_t mm = (seq_t)(m + 1);
                seq_t val = (mm * mm) / 4;
                if (mu > 0)
                    pos += val;
                else
                    neg += val;
            }
        }
    }

    /* pos ≥ neg because a(n) is a non‑negative count. */
    return pos - neg;
}
