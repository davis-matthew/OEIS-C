/*
 * A162325  a(n) = the largest divisor of n such that this and every smaller
 *          divisor of n are all coprime to each other.
 * https://oeis.org/A162325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor d of n such that d and all smaller
 *            divisors of n are pairwise coprime. For n = 1, a(1) = 1; for n
 *            >= 2 this is the largest divisor d < T, where T is the minimum
 *            of p^2 (for primes p with exponent >= 2) and p*q (for all
 *            distinct primes p, q dividing n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n <= 1ULL)
        return (seq_t)1;

    /* Factorise n. */
    idx_t tmp = n;
    idx_t primes[64];
    idx_t exps[64];
    size_t pcnt = 0;

    for (idx_t d = 2; d * d <= tmp; ++d)
    {
        if (tmp % d == 0)
        {
            idx_t e = 0;
            while (tmp % d == 0)
            {
                tmp /= d;
                ++e;
            }
            primes[pcnt] = d;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1)
    {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Compute threshold T. */
    idx_t T = n + 1;           /* initial value > n */
    for (size_t i = 0; i < pcnt; ++i)
    {
        if (exps[i] >= 2)
        {
            idx_t sq = primes[i] * primes[i];
            if (sq < T)
                T = sq;
        }
    }
    for (size_t i = 0; i < pcnt; ++i)
    {
        for (size_t j = i + 1; j < pcnt; ++j)
        {
            idx_t prod = primes[i] * primes[j];
            if (prod < T)
                T = prod;
        }
    }

    /* Find the largest divisor of n that is < T. */
    for (idx_t d = T - 1; d >= 1; --d)
    {
        if (n % d == 0)
            return (seq_t)d;
    }

    /* Fallback - should never happen for n > 1. */
    return (seq_t)n;
}
