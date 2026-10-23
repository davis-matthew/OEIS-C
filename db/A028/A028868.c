/*
 * A028868  Primes that when squared give numbers with digits in nonascending
 *          order.
 * https://oeis.org/A028868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   nonascending_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028868: primes whose squares have digits in non‑ascending order.
 *  Each element returned is the prime itself (not its square). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/nonascending_uint128.h"

/* Ensure an array has space for at least 'min_cap' elements.
 * If the array is NULL, it is allocated.  The pointer to the array
 * (as void*) is returned. */
static void *ensure_capacity(void *array, size_t elem_size, size_t *cap,
                             size_t min_cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < min_cap) new_cap <<= 1;
        array = realloc(array, new_cap * elem_size);
        *cap = new_cap;
    }
    return array;
}

/* Global state for incremental prime generation and caching. */
static uint64_t *prime_list = NULL;     /* primes used for primality tests  */
static size_t      prime_cap  = 0;
static size_t      prime_len  = 0;

static seq_t *cache      = NULL;       /* cached sequence terms (primes)  */
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;

/* Next integer to test for primality. Can be > 64‑bit on later calls,
 * but we stop early because the sequence is very sparse. */
static uint64_t next_candidate = 2;
static bool initialized = false;

/* Helper to initialise data structures once. */
static void init_state(void)
{
    if (initialized) return;
    prime_list = (uint64_t *)ensure_capacity(NULL, sizeof(uint64_t),
                                              &prime_cap, 16);
    prime_len  = 0;
    cache      = (seq_t *)ensure_capacity(NULL, sizeof(seq_t),
                                            &cache_cap, 16);
    cache_len  = 0;
    initialized = true;
}

/* Test whether cand is prime using the list of previously found primes.
 * The list is small (few thousands) so trial division is fast. */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break;          /* p*p would overflow cand */
        if (cand % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    init_state();

    /* Grow cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Advance to next candidate. */
        if (cand == 2) next_candidate = 3;
        else next_candidate = cand + 2;

        if (!is_prime_candidate(cand)) continue;   /* not a prime */

        /* Append prime to prime_list.  Ensure capacity before write. */
        prime_list = (uint64_t *)ensure_capacity(prime_list,
                                                 sizeof(uint64_t),
                                                 &prime_cap,
                                                 prime_len + 1);
        prime_list[prime_len++] = cand;

        /* Check digit property of cand^2. */
        unsigned __int128 sq128 = (unsigned __int128)cand * cand;
        if (nonascending_uint128(sq128)) {
            /* Append prime (not its square) to cache. */
            cache = (seq_t *)ensure_capacity(cache,
                                             sizeof(seq_t),
                                             &cache_cap,
                                             cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }

    /* Return the nth term (1‑based). */
    return cache[(size_t)n - 1];
}
