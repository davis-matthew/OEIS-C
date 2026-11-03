/*
 * A045415  Primes congruent to {1, 3, 5} mod 7.
 * https://oeis.org/A045415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that p mod 7 ∈ {1,3,5}
 * Range:     u8 n <= 27, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 62 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incrementally test each integer for primality (using
   is_prime from the utils library) and store qualifying primes in a static
   cache.  The cache is grown with ensure_cache so that calls with
   non‑increasing indices are answered in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Returns the n-th prime congruent to 1, 3, or 5 modulo 7 (n is 1‑based). */
seq_t a(idx_t n)
{
    /* Static cache and bookkeeping. */
    static seq_t *cache      = NULL;   /* cached terms               */
    static size_t cache_cap  = 0;       /* current cache capacity     */
    static size_t cache_len  = 0;       /* number of terms cached     */
    static idx_t next_start   = 2;      /* next integer to test       */

    /* Ensure the cache is large enough for n terms. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return quickly if we already have the result. */
    if (n <= cache_len)
        return cache[n-1];

    /* Generate primes until we have n terms cached. */
    while (cache_len < n) {
        /* Test successive integers for primality. */
        for (;;) {
            /* Skip even numbers after 2. */
            if (next_start > 2 && (next_start & 1) == 0) {
                ++next_start;
                continue;
            }

            if (is_prime(next_start)) {
                /* Check congruence modulo 7. */
                idx_t r = next_start % 7;
                if (r == 1 || r == 3 || r == 5) {
                    cache[cache_len++] = (seq_t)next_start;
                    ++next_start;
                    break;            /* exit for loop and check if we have enough */
                }
            }
            ++next_start;
        }
    }

    return cache[n-1];
}
