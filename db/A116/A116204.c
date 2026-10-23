/*
 * A116204  a(0) = 1; for n >= 1, a(n) = the number of positive divisors of n
 *          which are coprime to a(n-1).
 * https://oeis.org/A116204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = number of positive divisors of n that
 *            are coprime to a(n‑1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold entries up to index n (inclusive). */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap < (size_t)(n + 1))
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        /* cast to seq_t* so it compiles under C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);  /* out of memory – tests never trigger this */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Base case: a(0) = 1. */
    if (n == 0)
    {
        if (cache_len == 0)
        {
            ensure_cache_cap(0);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Compute any missing terms up to n. */
    if (n >= cache_len)
    {
        ensure_cache_cap(n);
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* Factor i. */
            uint64_t primes_i[64];
            uint64_t exps_i[64];
            size_t cnt_i;
            factor_u64((uint64_t)i, primes_i, exps_i, &cnt_i);

            /* Factor a(i‑1). */
            uint64_t primes_prev[64];
            uint64_t exps_prev[64];
            size_t cnt_prev;
            factor_u64((uint64_t)cache[i - 1], primes_prev, exps_prev, &cnt_prev);

            seq_t res = 1;
            for (size_t pi = 0; pi < cnt_i; ++pi)
            {
                bool coprime = true;
                for (size_t pj = 0; pj < cnt_prev; ++pj)
                {
                    if (primes_i[pi] == primes_prev[pj])
                    {
                        coprime = false;
                        break;
                    }
                }
                if (coprime)
                    res *= (seq_t)(exps_i[pi] + 1);
            }
            cache[i] = res;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
