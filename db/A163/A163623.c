/*
 * A163623  Primes of the form 120*k + 1.
 * https://oeis.org/A163623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime congruent to 1 mod 120 (search/sieve)
 * Range:     u8 n <= 1, u16 n <= 193, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_small, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 241; /* first prime 120*2 + 1 */

/* Ensure cache can hold at least `needed` entries.  Uses the
   `seq_alloc_array` helper to create properly-constructed arrays */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = seq_alloc_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    if (cache)
        seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term (n >= 1).  A value of 0 is returned for n == 0
   because the sequence has offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; undefined for n==0 */

    ensure_cache_capacity((size_t)n);

    while (cache_len < (size_t)n)
    {
        /* Find the next prime of the form 120*k + 1. */
        while (!is_prime_small((seq_t)next_candidate))
            next_candidate += 120;

        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate += 120;   /* next candidate will be 120*k + 1 again */
    }

    return cache[n - 1];
}
