/*
 * A161639  Positions n such that A010060(n) = A010060(n+8).
 * https://oeis.org/A161639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth element of the set of n such that parity(popcount(n)) ==
   parity(popcount(n+8)) */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of values stored */
static idx_t next_n = 0;             /* next integer to test */

seq_t a(idx_t n) {
    /* convert requested index to a size_t for array access */
    size_t target = (size_t)n;          /* n ≥ 1 per OEIS offset */

    /* Grow the cache until we have at least 'target' elements */
    while (cache_len < target) {
        /* Test whether parity(popcount(next_n)) == parity(popcount(next_n+8)) */
        unsigned long long ni = (unsigned long long)next_n;
        unsigned long long p1 = __builtin_popcountll(ni) & 1ULL;
        unsigned long long p2 = __builtin_popcountll(ni + 8ULL) & 1ULL;
        if (p1 == p2) {
            /* Ensure enough capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Out of memory – aborting is acceptable for this context */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_n;
        }
        ++next_n;
    }

    return cache[n - 1];
}
