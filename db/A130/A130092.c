/*
 * A130092  Numbers with at least two factors having in their canonical prime
 *          factorization equal exponents.
 * https://oeis.org/A130092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer whose canonical prime factorization
   contains at least two equal exponents.  The function builds the sequence on
   demand and memoises the results.  It works for seq_t being a small unsigned
   integer (uint8_t..uint64_t) or an arbitrary‑precision integer class. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Parameters for the cache.  CHUNK controls how many entries are added at a time. */
#define CHUNK 256

seq_t a(idx_t n)
{
    /* Increment n to 1‑based index; if n is 0 we simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Static cache of found numbers (stored as 64‑bit integers). */
    static uint64_t *cache = NULL;   /* underlying storage (to avoid object construction) */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t next_candidate = 2; /* next integer to test */

    /* Grow the cache until we have produced the nth term. */
    while (cache_len < (size_t)n) {

        uint64_t m = next_candidate;
        uint64_t x = m;          /* keep original value for storage */

        /* Gather exponents of prime factors in an array. */
        uint64_t exps[64];
        size_t exp_cnt = 0;

        /* Factor 2. */
        uint64_t e = 0;
        while (m % 2ULL == 0) {
            m /= 2ULL;
            ++e;
        }
        if (e > 0) exps[exp_cnt++] = e;

        /* Factor odd primes. */
        for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e > 0) exps[exp_cnt++] = e;
        }

        /* If anything remains, it is a prime factor with exponent 1. */
        if (m > 1ULL) exps[exp_cnt++] = 1ULL;

        /* Check for duplicate exponents. */
        bool dup = false;
        for (size_t i = 0; i < exp_cnt && !dup; ++i) {
            for (size_t j = i + 1; j < exp_cnt; ++j) {
                if (exps[i] == exps[j]) {
                    dup = true;
                    break;
                }
            }
        }

        /* If duplicate exponents were found, store the number. */
        if (dup) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap + CHUNK : CHUNK;
                uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }

        ++next_candidate;
    }

    /* Return the requested term, cast to seq_t. */
    return (seq_t)cache[n - 1];
}
