/*
 * A180098  Sigma(A180097(n)), sum of divisors of A180097(n), numbers n such
 *          that sigma(n) is powerful.
 * https://oeis.org/A180098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = σ(A180097(n)) where A180097(n) are numbers whose sum of *
 *            divisors is powerful (each prime factor appears with exponent ≥
 *            2)
 * Range:     u8 n <= 23, u16 n <= 1420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* static cache of computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values stored */
static size_t cache_cap = 0;   /* allocated capacity of cache array */
static idx_t next_candidate = 1;   /* next integer to test for property */

/* test whether a 64‑bit number is powerful */
static bool powerful_uint64(uint64_t m)
{
    if (m <= 1)          /* 1 is powerful by definition */
        return true;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] < 2)
            return false;

    return true;
}

/*  Main sequence generation function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* invalid index; return 0 as safe default */

    /* enlarge the cache until it contains the requested term */
    while (cache_len < n)
    {
        /* compute σ(candidate) */
        seq_t sigma = sigma_of(next_candidate);

        /* test whether σ(candidate) is powerful */
        uint64_t sigma64 = (uint64_t)sigma;   /* safe for the required n */
        if (powerful_uint64(sigma64))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? 2 * cache_cap : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failed */
                {
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sigma;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
