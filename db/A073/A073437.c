/*
 * A073437  Smallest x such that remainder Mod[A065855(x), A000720(x)]=n.
 * https://oeis.org/A073437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest x ≥ 1 such that (x - π(x) - 1) mod π(x) = n,
 *            where π(x) is the number of primes ≤ x
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The helper types seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static bool initialized = false;
    static idx_t *primes = NULL;      /* dynamic array of found primes */
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    static seq_t *cache = NULL;       /* cache of answers a[1..max_needed] */
    static size_t cache_size = 0;
    static size_t max_needed = 0;     /* largest n we have processed so far */
    static size_t remaining = 0;      /* how many indices 1..max_needed are still 0 */
    static idx_t current_x = 1;       /* last x examined */

    if (!initialized) {
        initialized = true;
        /* allocate minimal space for the cache */
        cache_size = 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t)); /* cache[0] unused */
        if (!cache) return 0; /* allocation failure, return 0 as a sentinel */
    }

    /* Ensure the cache can hold up to n */
    if (n > max_needed) {
        /* enlarge the cache */
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        /* zero‑initialize the new part */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
        /* update bookkeeping */
        remaining += (size_t)(n - max_needed);
        max_needed = n;
    }

    /* Scan successive integers x until all remainders up to max_needed have been found */
    while (remaining > 0) {
        ++current_x;

        /* Test whether current_x is prime by trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            /* Stop if p * p > current_x to avoid unnecessary checks */
            if (p * p > current_x) break;
            if (current_x % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* Append new prime to the list */
            if (primes_cnt + 1 > primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                idx_t *new_primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!new_primes) return 0; /* allocation failure */
                primes = new_primes;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = current_x;
        }

        /* Number of primes ≤ current_x */
        idx_t pi = (idx_t)primes_cnt;
        if (pi == 0) continue;                     /* skip x < 2, though this never happens */

        /* Compute the remainder: (current_x - pi - 1) mod pi */
        seq_t tmp = (seq_t)(current_x - pi - 1);
        seq_t rem = tmp % (seq_t)pi;                /* 0 … pi-1 */

        /* Store the answer if this is the first occurrence of this remainder */
        size_t r = (size_t)rem;
        if (r > 0 && r <= max_needed && cache[r] == 0) {
            cache[r] = current_x;
            --remaining;
        }
    }

    return cache[(size_t)n];
}
