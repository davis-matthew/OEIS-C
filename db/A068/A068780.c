/*
 * A068780  Composite numbers n such that n+1 is also composite.
 * https://oeis.org/A068780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068780: composite numbers n such that n+1 is also composite */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t last_checked = 1;       /* last integer examined for the pair */

static void grow_cache(idx_t target) {
    while (cache_size < (size_t)target) {
        last_checked++;   /* move to next integer */
        /* both n and n+1 must be composite */
        if (!is_prime_small((seq_t)last_checked) &&
            !is_prime_small((seq_t)(last_checked + 1))) {
            /* ensure we have space for a new entry */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */
    /* grow cache until it contains at least n terms */
    if (cache_size < (size_t)n) grow_cache(n);
    return cache[n - 1];
}
