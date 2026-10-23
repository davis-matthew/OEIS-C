/*
 * A045447  Primes congruent to {0, 5} mod 7.
 * https://oeis.org/A045447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 815, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth prime congruent to 0 or 5 modulo 7.
   The first two terms are 5 and 7; thereafter we list primes ≡5 (mod 7). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of cached terms                     */
static size_t cache_cnt = 0;         /* number of terms stored so far             */
static size_t cache_cap = 0;         /* current capacity of the array             */
static bool   initialized = false;   /* have we performed one‑time initialisation? */
static uint64_t next_candidate = 9;   /* odd candidate after the initial terms */

/* Ensure the cache array can hold at least 'need' elements.
   Uses the utility 'grow_seq' – it reallocates while preserving contents. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need)
        grow_seq(&cache, &cache_cap, need);
}

/* One‑time initialisation of the cache with the first two terms. */
static void init_cache(void)
{
    if (initialized) return;

    /* Allocate initial capacity and insert the first two known terms. */
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();                  /* out of memory */
    cache[0] = (seq_t)5;
    cache[1] = (seq_t)7;
    cache_cnt = 2;

    /* The next odd number to test for being prime. */
    next_candidate = 9;
    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();

    /* Extend the cache until it contains the nth term. */
    while (cache_cnt < n)
    {
        ensure_cache_capacity(cache_cnt + 1);

        /* Find the next prime congruent to 5 (mod 7). */
        for (;;)
        {
            uint64_t p = next_candidate;
            next_candidate = p + 2;              /* skip to the next odd candidate */

            if (is_prime_big((seq_t)p) && (p % 7ULL == 5ULL))
            {
                cache[cache_cnt++] = (seq_t)p;
                break;   /* term added – proceed to generate further terms if needed */
            }
            /* otherwise keep searching */
        }
    }

    return cache[n - 1];   /* idx_t is 1‑based, cache is 0‑based */
}
