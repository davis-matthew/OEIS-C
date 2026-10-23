/*
 * A138404  a(n) = prime(n)^5 - prime(n).
 * https://oeis.org/A138404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^5 - prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 1 (offset 1). */

/* This implementation builds a small prime cache lazily using trial division.
   For n <= 10000 the 10000th prime is 104729, so the cache never grows
   beyond that.  A simple incremental sieve would also work, but trial
   division is sufficient and keeps the code short.  The cache is thread-
   safe only for single‑threaded use; the test harness is single‑threaded. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

/* ---------- Prime cache management ---------- */
static idx_t *prime_cache      = NULL; /* 1‑based array of primes */
static size_t prime_cache_size = 0;    /* current capacity */
static size_t prime_cache_cnt  = 0;    /* number of primes stored */

/* Ensure that prime_cache contains at least 'needed' primes. */
static void ensure_primes(idx_t needed)
{
    if (prime_cache_cnt >= needed)
        return;

    /* Expand cache capacity if necessary. */
    if (prime_cache_size < needed)
    {
        size_t new_size = needed > prime_cache_size * 2 ? needed : prime_cache_size * 2;
        if (new_size < 4)
            new_size = 4;
        idx_t *tmp = (idx_t *)realloc(prime_cache, new_size * sizeof(idx_t));
        if (tmp == NULL) {
            /* Fatal error: memory allocation failed */
            exit(EXIT_FAILURE);
        }
        prime_cache = tmp;
        prime_cache_size = new_size;
    }

    /* Find successive primes until we have 'needed' of them. */
    idx_t candidate = (prime_cache_cnt == 0) ? 2
                                              : prime_cache[prime_cache_cnt - 1] + 1;
    while (prime_cache_cnt < needed)
    {
        bool is_prime = true;
        /* trial division by known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_cache_cnt && prime_cache[i] * prime_cache[i] <= candidate; ++i)
        {
            if (candidate % prime_cache[i] == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            prime_cache[prime_cache_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Return the n‑th prime (1‑based).  Caller must ensure n >= 1. */
static idx_t nth_prime(idx_t n)
{
    ensure_primes(n);
    return prime_cache[n - 1];
}

/* ---------- Sequence implementation ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; return 0 for safety when n==0 */
        return (seq_t)0;

    idx_t p = nth_prime(n);                    /* 1‑based prime */
    seq_t p_seq = (seq_t)p;
    seq_t p5 = pow_uint((uint64_t)p, (idx_t)5); /* p^5 */
    return p5 - p_seq;                         /* prime^5 - prime */
}
