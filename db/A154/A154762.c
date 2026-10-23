/*
 * A154762  Primes with the same number of decimal digits that are {1, 9} as
 *          {2, 3, 5, 7}.
 * https://oeis.org/A154762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence a(n) = nth prime p such that count of prime decimal
 *            digits {2,3,5,7} in p equals count of odd non‑prime decimal
 *            digits {1,9} in p
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   valid_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a dynamic sieve and incremental
    generation of terms.  All data structures are static
    so multiple calls to a(idx_t n) are handled correctly.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/valid_prime.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour below offset is irrelevant */

    /* static state */
    static seq_t *cache = NULL;          /* stored valid primes */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint32_t *primes = NULL;      /* all primes found so far */
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    static uint8_t *is_prime = NULL;     /* sieve bitmap (1 byte per num) */
    static size_t sieve_cap = 0;
    static size_t sieve_limit = 0;       /* current processed limit */

    static size_t proc_idx = 0;           /* index of next prime to examine */

    /* ensure cache big enough */
    if (cache_cap < (size_t)n)
    {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* if we need more primes, extend the sieve */
        if (proc_idx >= primes_cnt)
        {
            size_t new_limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
            /* adjust sieve array */
            if (sieve_cap < new_limit + 1)
            {
                size_t new_cap = new_limit + 1;
                uint8_t *tmp = (uint8_t*)realloc(is_prime, new_cap * sizeof(uint8_t));
                if (!tmp) exit(1);
                is_prime = tmp;
                sieve_cap = new_cap;
            }
            /* initialise new section as prime */
            if (sieve_limit == 0)
            {
                is_prime[0] = 0;
                is_prime[1] = 0;
                for (size_t i = 2; i <= new_limit; ++i) is_prime[i] = 1;
            }
            else
            {
                memset(is_prime + sieve_limit + 1, 1,
                       new_limit - sieve_limit);
            }

            /* mark composites using existing primes up to sqrt(new_limit) */
            for (size_t i = 0; i < primes_cnt; ++i)
            {
                uint32_t p = primes[i];
                if ((uint64_t)p * p > new_limit) break;
                size_t start = (sieve_limit + 1 + p - 1) / p * p;
                for (size_t j = start; j <= new_limit; j += p)
                    is_prime[j] = 0;
            }

            /* discover new primes and add to list */
            for (uint32_t i = (uint32_t)(sieve_limit + 1); i <= (uint32_t)new_limit; ++i)
            {
                if (is_prime[i])
                {
                    /* extend primes array if needed */
                    if (primes_cap <= primes_cnt)
                    {
                        size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
                        uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                        if (!tmp) exit(1);
                        primes = tmp;
                        primes_cap = new_cap;
                    }
                    primes[primes_cnt++] = i;

                    /* mark its multiples for future range */
                    if ((uint64_t)i * i <= new_limit)
                    {
                        for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                            is_prime[j] = 0;
                    }
                }
            }
            sieve_limit = new_limit;
        }

        /* process next prime */
        uint32_t p = primes[proc_idx++];
        if (valid_prime(p))
            cache[cache_len++] = p;
    }

    return cache[n - 1];
}
