/*
 * A199977  Primes whose multiplicative digital root is 0.
 * https://oeis.org/A199977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 4744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mdr_zero, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute A199977 – Primes whose multiplicative digital root is 0.
 * The function searches for primes, checks whether their repeated
 * digit‑product eventually becomes 0, and returns the nth such prime.
 * 
 * This implementation uses a simple incremental search with the
 * supplied next_prime helper.  It is O(n log n) and runs quickly for
 * n up to at least 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mdr_zero.h"

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Internal cache of found terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Ensure the cache has capacity for at least `n` additional slots */
static void cache_ensure(size_t n)
{
    if (cache_len + n > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        while (new_cap < cache_len + n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Append a prime to the cache */
static void cache_append(seq_t val)
{
    cache_ensure(1);
    cache[cache_len++] = val;
}

/*
 * Return the nth term of A199977 (1‑based indexing).
 * The function lazily generates primes and evaluates them until the
 * requested index is satisfied.  It is safe for arbitrary ordering
 * of calls.
 */
seq_t a(idx_t n)
{
    /*  n is idx_t (unsigned 64‑bit), but indices we use for
        cached array are size_t.  All indices are positive.     */
    while (cache_len < (size_t)n) {
        static uint64_t next_start = 2;   /* maintain search start across calls */
        uint64_t candidate = next_prime(next_start);
        next_start = candidate + 1;

        if (mdr_zero(candidate)) {
            /* Store the prime as seq_t (truncated if necessary) */
            cache_append((seq_t)candidate);
        }
    }
    return cache[n - 1];
}
