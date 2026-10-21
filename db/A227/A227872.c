/*
 * A227872  Number of odious divisors (A000069) of n.
 * https://oeis.org/A227872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} A010060(d) (odious divisors of n)
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_odd.h"

/* recursive enumeration of all divisors from the factorisation data
   primes[0..factors-1] with exponents exps[0..factors-1] */
static void enumerate_divisors(int idx, seq_t current,
                               uint64_t *primes, int *exps, int factors,
                               seq_t *odious_count)
{
    if (idx == factors)
    {
        if (popcount_odd(current))
            (*odious_count)++;
        return;
    }

    seq_t temp = current;
    for (int e = 0; e <= exps[idx]; ++e)
    {
        enumerate_divisors(idx + 1, temp, primes, exps, factors, odious_count);
        temp = temp * (seq_t)primes[idx];
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;

    /* factorisation: at most 64 distinct primes for 64‑bit N */
    uint64_t primes[64];
    int     exps[64];
    int     factors = 0;

    uint64_t temp = N;
    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            do
            {
                temp /= p;
                exp++;
            } while (temp % p == 0);

            primes[factors] = p;
            exps[factors]   = exp;
            factors++;
        }
    }
    if (temp > 1)
    {
        primes[factors] = temp;
        exps[factors]   = 1;
        factors++;
    }

    seq_t odious = 0;
    enumerate_divisors(0, (seq_t)1, primes, exps, factors, &odious);
    return odious;
}
