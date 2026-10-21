/*
 * A084190  Least common multiple of {d-1: d > 1 and d divides n}.
 * https://oeis.org/A084190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{d-1 : d>1 and d|n}
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 107, u64 n <= 359,
 *            u128 n <= 1439, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor.h"

/* Small, type‑generic gcd for seq_t (works for all provided integer types) */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursive enumeration of all divisors > 1 of n.
   cur    – current divisor being built (fits in uint64_t)
   idx    – current prime factor index
   pr     – array of prime factors of n
   exp    – array of corresponding exponents
   cnt    – number of distinct prime factors
   lcm    – pointer to the current lcm value (seq_t)                */
static void enumerate_divisors(uint64_t cur, int idx,
                               const uint64_t *pr, const uint64_t *exp,
                               int cnt, seq_t *lcm)
{
    if (idx == cnt)
    {
        if (cur > 1)
        {
            seq_t dm = (seq_t)(cur - 1);
            seq_t g  = seq_gcd(*lcm, dm);
            *lcm = (*lcm / g) * dm;
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t pw = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e)
    {
        enumerate_divisors(cur * pw, idx + 1, pr, exp, cnt, lcm);
        pw *= p;
    }
}

seq_t a(idx_t n)
{
    seq_t lcm_val = 1;

    /* Factorise n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor((uint64_t)n, primes, exps, &factor_cnt);

    /* Enumerate all divisors > 1 and accumulate their (d‑1) LCM */
    enumerate_divisors(1ULL, 0, primes, exps, (int)factor_cnt, &lcm_val);

    return lcm_val;
}
