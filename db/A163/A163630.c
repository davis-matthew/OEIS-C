/*
 * A163630  Numbers such that the two adjacent integers are a prime and 7
 *          times a prime.
 * https://oeis.org/A163630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 372, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer m such that either
 *  (m-1) is prime and (m+1) = 7×prime
 *  or
 *  (m-1) = 7×prime and (m+1) is prime
 * Sequence index starts at 1 (a(1)=20). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache     = NULL;   /* cached terms */
static idx_t  cache_len = 0;      /* number of cached terms */
static idx_t  cache_cap = 0;      /* allocated cache capacity */
static idx_t  next_m    = 2;      /* next integer to test */

/* Append a value to the cache, resizing if necessary. */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; callers should use n>=1 */

    while (cache_len < n) {
        bool ok = false;

        /* condition: (m-1) is prime and (m+1) = 7×prime */
        if (is_prime(next_m - 1) && ((next_m + 1) % 7 == 0) &&
            is_prime((next_m + 1) / 7))
            ok = true;

        /* condition: (m-1) = 7×prime and (m+1) is prime */
        if (!ok && ((next_m - 1) % 7 == 0) &&
            is_prime((next_m - 1) / 7) && is_prime(next_m + 1))
            ok = true;

        if (ok)
            push_cache((seq_t)next_m);

        ++next_m;
    }

    return cache[n - 1];
}
