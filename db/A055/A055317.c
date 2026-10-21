/*
 * A055317  Number of labeled trees with n nodes and 5 leaves.
 * https://oeis.org/A055317
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-5)^2 * (n-4)^2 * (n-3) * (n-2) / 5760 (valid for
 *            n >= 6; for n < 6 the function returns 0)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Multiply primitive integer powers into a seq_t value */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;  /* undefined below offset */

    /* Simple sieve to find all primes up to n */
    int limit = (int)n;
    /* Allocate array for prime flags */
    int *is_prime = (int *)calloc(limit + 1, sizeof(int));
    if (!is_prime)
        return 0;  /* allocation failure */

    for (int i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    for (int i = 2; (int64_t)i * i <= limit; ++i)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    int primes_cnt = 0;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++primes_cnt;

    int *primes = (int *)malloc(primes_cnt * sizeof(int));
    if (!primes)
    {
        free(is_prime);
        return 0;
    }

    int idx = 0;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = i;

    free(is_prime);

    /* Exponents of each prime in the final result */
    uint64_t *exp_fact = (uint64_t *)calloc(primes_cnt, sizeof(uint64_t));
    if (!exp_fact)
    {
        free(primes);
        return 0;
    }

    /* Exponents coming from n! (factorial part) */
    for (int pi = 0; pi < primes_cnt; ++pi)
    {
        int p = primes[pi];
        uint64_t e = 0;
        idx_t t = n;
        while (t)
        {
            t /= (idx_t)p;
            e += (uint64_t)t;
        }
        exp_fact[pi] = e;
    }

    /* Extra multiplicative terms: (n-5)^2 * (n-4)^2 * (n-3) * (n-2) */
    int values[6] = {
        (int)(n - 5),
        (int)(n - 5),
        (int)(n - 4),
        (int)(n - 4),
        (int)(n - 3),
        (int)(n - 2)
    };

    for (int k = 0; k < 6; ++k)
    {
        int x = values[k];
        for (int pi = 0; pi < primes_cnt && (int64_t)primes[pi] * primes[pi] <= x; ++pi)
        {
            int p = primes[pi];
            if (x % p == 0)
            {
                int cnt = 0;
                while (x % p == 0)
                {
                    x /= p;
                    ++cnt;
                }
                exp_fact[pi] += (uint64_t)cnt;
            }
        }
        if (x > 1) /* x itself is prime */
        {
            /* find index of x in primes array */
            for (int pi = 0; pi < primes_cnt; ++pi)
            {
                if (primes[pi] == x)
                {
                    exp_fact[pi] += 1;
                    break;
                }
            }
        }
    }

    /* Subtract denominator 5760 = 2^7 * 3^2 * 5^1 */
    for (int pi = 0; pi < primes_cnt; ++pi)
    {
        if (primes[pi] == 2)
            exp_fact[pi] -= 7;
        else if (primes[pi] == 3)
            exp_fact[pi] -= 2;
        else if (primes[pi] == 5)
            exp_fact[pi] -= 1;
    }

    /* Build the result from prime powers */
    seq_t result = 1;
    for (int pi = 0; pi < primes_cnt; ++pi)
    {
        uint64_t e = exp_fact[pi];
        if (e > 0)
            result = result * pow_seq((seq_t)primes[pi], e);
    }

    free(primes);
    free(exp_fact);
    return result;
}
