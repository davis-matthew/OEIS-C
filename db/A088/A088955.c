/*
 * A088955  Primes of the form 60*k + 1.
 * https://oeis.org/A088955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 60*k + 1 (OEIS A088955)
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* cache of found primes 1 (mod 60) */
static seq_t *cache = NULL;          /* malloced array of seq_t */
static size_t cache_len = 0;         /* number of cached elements */
static size_t cache_cap = 0;         /* allocated capacity */
static uint64_t next_candidate = 61; /* next number 60*k+1 to test */

/* ensure the cache has space for 'needed' elements */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* grow the cache until it contains at least 'n' elements */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;
    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)next_candidate)) {
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 60;
    }
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* for indices below the offset */
    ensure_cache(n);
    return cache[n - 1];
}
