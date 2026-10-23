/*
 * A183096  a(n) = number of divisors of n that are not perfect powers.
 * https://oeis.org/A183096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not perfect powers
 * Range:     u8 n <= 9551, u16 n <= 9551, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* helper to compute gcd of uint64_t values */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* recursive enumeration of all divisors of n */
static void dfs_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, size_t idx,
                         uint64_t cur, uint64_t cur_gcd,
                         seq_t *non_pp)
{
    if (idx == cnt)
    {
        /* cur is a divisor of n.
           It is a perfect power iff cur==1 or cur_gcd >= 2. */
        if (cur != 1 && cur_gcd < 2)
            (*non_pp)++;   /* not a perfect power */
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t emax = exps[idx];
    uint64_t val = 1;   /* p^e for current e */

    for (uint64_t e = 0; e <= emax; ++e)
    {
        uint64_t next_gcd;
        if (e == 0)
            next_gcd = cur_gcd;               /* no change */
        else if (cur_gcd == 0)
            next_gcd = e;                     /* first non‑zero exponent */
        else
            next_gcd = gcd_uint64(cur_gcd, e);

        dfs_divisors(primes, exps, cnt, idx + 1,
                     cur * val, next_gcd, non_pp);
        val *= p;
    }
}

/* main function requested by the build */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;   /* 1 has one divisor (1) which is a perfect power */

    uint64_t nn = (uint64_t)n;

    /* factor n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* factor 2 separately */
    if ((nn & 1ULL) == 0)
    {
        uint64_t e = 0;
        while ((nn & 1ULL) == 0)
        {
            nn >>= 1;
            e++;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        cnt++;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= nn; p += 2)
    {
        if (nn % p == 0)
        {
            uint64_t e = 0;
            while (nn % p == 0)
            {
                nn /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }
    if (nn > 1)   /* remaining prime factor */
    {
        primes[cnt] = nn;
        exps[cnt]   = 1;
        cnt++;
    }

    seq_t non_pp = 0;
    dfs_divisors(primes, exps, cnt, 0, 1ULL, 0, &non_pp);
    return (seq_t)non_pp;
}
