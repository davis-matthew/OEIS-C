/*
 * A179208  Primes prime(k) such that 2*prime(k) = prime(k-2) + prime(k+2).
 * https://oeis.org/A179208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incrementally generate primes and test the condition
   2*prime(k) == prime(k-2)+prime(k+2).  a(1) returns the first such prime. */

#include <stdlib.h>
#include <stdbool.h>

/* check whether n is prime using the primes already known */
static bool is_prime_uint64(uint64_t n, const uint64_t *primes, size_t count)
{
    if (n < 2u) return false;
    if (n == 2u) return true;
    if ((n & 1u) == 0u) return false;
    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0u) return false;
    }
    return true;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* static state for prime storage and result cache */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0, primes_cnt = 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_cnt = 0;

    static uint64_t next_candidate = 5;   /* start from 5 (2 and 3 are already stored) */
    static bool initialized = false;

    if (!initialized) {
        primes_cap = 1024;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

        /* the first two primes */
        primes[0] = 2;
        primes[1] = 3;
        primes_cnt = 2;

        initialized = true;
    }

    /* extend cache until we have the requested term */
    while (cache_cnt < (size_t)n) {
        /* find the next prime */
        uint64_t p;
        for (;;) {
            if (next_candidate < 2u) next_candidate = 2u;
            if (is_prime_uint64(next_candidate, primes, primes_cnt)) {
                p = next_candidate;
                break;
            }
            next_candidate += 2u;               /* skip even numbers */
            if (next_candidate < 3u)            /* overflow guard */
                next_candidate = 3u;
        }

        /* store the new prime, resizing if necessary */
        if (primes_cnt >= primes_cap) {
            primes_cap <<= 1;
            uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes = tmp;
        }
        primes[primes_cnt++] = p;

        /* advance candidate for the next iteration */
        next_candidate += 2u;
        if (next_candidate < 5u) next_candidate = 5u; /* overflow guard */

        /* when we have at least five primes, test the prime two places back */
        if (primes_cnt >= 5) {
            /* idx is 1‑based index of k = primes_cnt‑2 (last prime two positions back)   */
            size_t idx = primes_cnt - 2;            /* primes indices are 0‑based */
            uint64_t pk    = primes[idx - 1];       /* prime(k)          */
            uint64_t pk_m2 = primes[idx - 3];       /* prime(k-2)        */
            uint64_t pk_p2 = primes[idx + 1];       /* prime(k+2)        */

            if (2 * pk == pk_m2 + pk_p2) {
                if (cache_cnt >= cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache[cache_cnt++] = (seq_t) pk;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
