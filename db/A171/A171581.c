/*
 * A171581  The natural numbers without primes > 3.
 * https://oeis.org/A171581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: natural numbers that are not prime or are prime <= 3
   (i.e. 1, 2, 3, and all composite numbers). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Grow a dynamically allocated array of seq_t values.  Works in C and C++. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Cache to store the sequence values. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t  next_candidate;   /* next integer to examine */

/* Initialise the cache and the first candidate value. */
static void init_cache(void)
{
    if (cache) return;            /* already initialised */
    cache        = NULL;
    cache_len    = 0;
    cache_cap    = 0;
    next_candidate = (seq_t)1;     /* start from 1 */
}

/* Ensure that at least N terms are present in the cache. */
static void ensure_cache(size_t N)
{
    if (cache_len >= N) return;

    init_cache();
    grow_seq(&cache, &cache_cap, N);

    while (cache_len < N)
    {
        /* Skip primes greater than 3. */
        if (next_candidate > (seq_t)3 && is_prime_big(next_candidate))
        {
            next_candidate = next_candidate + (seq_t)1;
            continue;
        }

        /* Accept the current candidate. */
        cache[cache_len++] = next_candidate;
        next_candidate = next_candidate + (seq_t)1;
    }
}

/* The required OEIS function. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    size_t N = (size_t)n;   /* for array indexing */
    ensure_cache(N);
    return cache[N - 1];
}
