/*
 * A081727  Length of periods of Euler numbers modulo n.
 * https://oeis.org/A081727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Length of periods of Euler numbers modulo n.
   The algorithm uses the known factorisation of the period:
   * For an odd prime power p^e:
        period(p^e) = period(p) * p^(e-1)
        period(p) = p-1 if p ≡ 3 (mod 4)
                  = (p-1)/2 if p ≡ 1 (mod 4)
   * For powers of two:
        period(1) = 1, period(2) = 1, period(4) = 1,
        period(2^k) = 2^(k-1) for k ≥ 3.
   The period for a composite n is the least common multiple of the
   prime‑power periods. */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factorise n */
    idx_t temp = n;
    idx_t limit = (idx_t)sqrt((double)n);

    idx_t *primes = NULL;        /* primes up to sqrt(n) */
    size_t primes_cap = 0;
    build_primes(limit, &primes, &primes_cap);

    seq_t period = (seq_t)1;

    for (size_t i = 0; i < primes_cap && primes[i] <= limit; ++i)
    {
        idx_t p = primes[i];
        if (temp % p != 0)
            continue;

        /* exponent of p in n */
        idx_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }

        seq_t factor_period = 1;

        if (p == 2)
        {
            if (exp <= 2)
                factor_period = (seq_t)1;                /* 2^1, 2^2 → period 1 */
            else
            {
                seq_t pp = 1;
                for (idx_t k = 0; k < exp - 1; ++k)
                    pp *= (seq_t)2;                    /* 2^(exp-1) */
                factor_period = pp;
            }
        }
        else
        {
            seq_t base = (p % 4 == 3) ? (seq_t)(p - 1)
                                      : (seq_t)((p - 1) / 2);
            if (exp == 1)
                factor_period = base;
            else
            {
                seq_t pp = 1;
                for (idx_t k = 0; k < exp - 1; ++k)
                    pp *= (seq_t)p;                    /* p^(exp-1) */
                factor_period = base * pp;
            }
        }

        /* lcm with current period */
        seq_t g = gcd(period, factor_period);
        period = (period / g) * factor_period;
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1)
    {
        idx_t p = temp;          /* exponent is 1 */
        seq_t factor_period = 1;

        if (p == 2)
        {
            /* p==2 already handled above; but for safety */
            factor_period = (seq_t)1;
        }
        else
        {
            seq_t base = (p % 4 == 3) ? (seq_t)(p - 1)
                                      : (seq_t)((p - 1) / 2);
            factor_period = base;   /* exp == 1 */
        }

        seq_t g = gcd(period, factor_period);
        period = (period / g) * factor_period;
    }

    free(primes);
    return period;
}
