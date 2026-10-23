/*
 * A045463  Primes congruent to {0, 1, 6} mod 7.
 * https://oeis.org/A045463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045463: primes ≡ 0, 1 or 6 (mod 7), offset 1 – obtained by incremental search. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* cached primes */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of cached primes */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* safety guard – offset starts at 1 */

    /* Return a cached value if possible. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Start searching from the integer following the last cached prime. */
    idx_t cand = (cache_len > 0)
                 ? (idx_t)(cache[cache_len - 1] + 1)
                 : (idx_t)2;

    /* Find primes until we reach the desired index. */
    while (cache_len < n) {
        if (cand == 7 || cand % 7 == 1 || cand % 7 == 6) {
            if (is_prime_big((seq_t)cand)) {
                /* Grow the cache if needed. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len] = (seq_t)cand;
                ++cache_len;
            }
        }
        ++cand;
    }

    return cache[n - 1];
}
