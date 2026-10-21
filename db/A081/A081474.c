/*
 * A081474  Number of distinct lines through the origin in n-dimensional cube
 *          of side length n.
 * https://oeis.org/A081474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^n μ(d) * ( (⌊n/d⌋+1)^n - 1 ) (μ is the Möbius
 *            function)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef int8_t mobius_t;

/* multiply base^exp where exp >= 0; works for any seq_t type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e)
    {
        if (e & 1LL)
            result *= b;
        b *= b;
        e >>= 1LL;
    }
    return result;
}

/* compute μ(d) for 1..limit; result in static array mu[0..limit]   */
static void compute_mobius(idx_t limit, mobius_t **mu_out)
{
    static idx_t cur_limit = 0;
    static mobius_t *mu   = NULL;
    static uint32_t *primes = NULL;
    static size_t  prime_cnt = 0;
    static size_t  prime_cap = 0;

    if (limit > cur_limit)
    {
        size_t new_len = limit + 1;
        mu = (mobius_t*)realloc(mu, new_len * sizeof(mobius_t));
        uint32_t *lp = (uint32_t*)calloc(new_len, sizeof(uint32_t));
        memset(mu, 0, new_len * sizeof(mobius_t));
        mu[1] = 1;

        for (idx_t i = 2; i <= limit; ++i)
        {
            if (lp[i] == 0)                /* i is prime */
            {
                lp[i] = (uint32_t)i;
                mu[i] = -1;
                if (prime_cap == prime_cnt)
                {
                    prime_cap = prime_cap ? prime_cap << 1 : 4;
                    primes = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                }
                primes[prime_cnt++] = (uint32_t)i;
            }
            for (size_t k = 0; k < prime_cnt && (idx_t)primes[k]*i <= limit; ++k)
            {
                idx_t p  = primes[k];
                idx_t ip = i * p;
                lp[ip] = (uint32_t)p;
                if (i % p == 0)          /* p divides i */
                {
                    mu[ip] = 0;
                    break;
                }
                else
                {
                    mu[ip] = -mu[i];
                }
            }
        }
        free(lp);
        cur_limit = limit;
    }
    *mu_out = mu;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    mobius_t *mu = NULL;
    compute_mobius(n, &mu);

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        mobius_t md = mu[d];
        if (md == 0)
            continue;

        idx_t factor = n / d + 1;                     /* ⌊n/d⌋+1  */
        seq_t powval = pow_seq((seq_t)factor, n);     /* (⌊n/d⌋+1)^n */
        powval -= 1;                                  /* subtract 1   */
        if (md > 0)
            sum += powval;
        else
            sum -= powval;
    }
    return sum;
}
