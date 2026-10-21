/*
 * A089229  Neither primes nor square numbers.
 * https://oeis.org/A089229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite integer that is not a perfect square
 *            (i.e., numbers that are neither prime nor square). This matches
 *            OEIS A089229
 * Range:     u8 n <= 186, u16 n <= 8671, u32 n <= 8671, u64 n <= 8671,
 *            u128 n <= 8671, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Append a value to the cache, growing the array if needed. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Generate composites that are not squares until the cache holds at least n terms. */
static void ensure_up_to(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= n) return;

    idx_t candidate = cache_len == 0 ? 2 : ((idx_t)cache[cache_len - 1] + 1);
    while (cache_len < n) {
        /* Skip 0 and 1 explicitly. */
        if (candidate <= 1) { candidate = 2; continue; }

        /* Quick composite check: not a prime. */
        if (!is_prime(candidate)) {
            /* Check if candidate is a perfect square. */
            idx_t r = int_sqrt(candidate);
            if ((seq_t)r * (seq_t)r != candidate) {
                cache_append((seq_t)candidate);
            }
        }
        ++candidate;
    }
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n - 1];
}
