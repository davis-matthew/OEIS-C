/*
 * A189805  Numerators of coefficients in the series expansion of ((2 - m)
 *          EllipticK(m) - 2 EllipticE(m))/(Pi * m).
 * https://oeis.org/A189805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ((2n-1)!!)^2/(2^(2n+1)*(n-1)!*(n+1)!) after
 *            cancelling common factors. The subscript n starts at 0: a(0)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

#define MAX_PRIME 20000          /* enough for 2n-1 when n ≤ 10000 */

static idx_t primes[MAX_PRIME + 1];
static idx_t prime_cnt = 0;

/* Simple sieve of Eratosthenes to generate all primes ≤ MAX_PRIME. */
static void sieve(void)
{
    if (prime_cnt != 0)
        return;

    bool composite[MAX_PRIME + 1] = {false};
    for (idx_t p = 2; p <= MAX_PRIME; ++p)
    {
        if (!composite[p])
        {
            primes[prime_cnt++] = p;
            if (p * p <= MAX_PRIME)
                for (idx_t q = p * p; q <= MAX_PRIME; q += p)
                    composite[q] = true;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    sieve();

    /* Compute exponent of each prime p in the full fraction numerator minus denominator. */
    int *exp_arr = (int *)calloc(prime_cnt, sizeof(int));
    if (!exp_arr)
        return (seq_t)0;          /* memory error – unlikely in test harness */

    /* Numerator: ((2n-1)!!)^2 => product of squares of all odd numbers <= 2n-1. */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t num = 2 * k - 1;    /* odd number */
        idx_t temp = num;

        /* factor num, add twice the exponent */
        for (idx_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i)
        {
            idx_t p = primes[i];
            int cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                cnt++;
            }
            if (cnt > 0)
                exp_arr[i] += 2 * cnt;
        }
        if (temp > 1)              /* temp itself a prime */
        {
            for (idx_t i = 0; i < prime_cnt; ++i)
                if (primes[i] == temp)
                {
                    exp_arr[i] += 2;
                    break;
                }
        }
    }

    /* Denominator: 2^(2n+1). */
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        if (primes[i] == 2)
        {
            exp_arr[i] -= (int)(2 * n + 1);
            break;
        }
    }

    /* Denominator: (n-1)! and (n+1)! (Legendre's formula). */
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];
        idx_t power = p;
        int sub = 0;

        while (power <= n - 1)
        {
            sub += (int)((n - 1) / power);
            if (power > (UINT64_MAX / p))
                break;
            power *= p;
        }
        power = p;
        while (power <= n + 1)
        {
            sub += (int)((n + 1) / power);
            if (power > (UINT64_MAX / p))
                break;
            power *= p;
        }
        exp_arr[i] -= sub;
    }

    /* Build the numerator by multiplying primes with positive exponent. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        int e = exp_arr[i];
        if (e > 0)
        {
            seq_t term = seq_pow((seq_t)primes[i], (idx_t)e);
            result = seq_mul(result, term);
        }
    }

    free(exp_arr);
    return result;
}
