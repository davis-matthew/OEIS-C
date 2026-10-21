/*
 * A075747  Numbers n such that 210*n + 17 is prime.
 * https://oeis.org/A075747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th non‑negative integer m such that 210*m+17 is
 *            prime (offset 1: a(1)=0). The implementation searches candidates
 *            sequentially and caches found terms for arbitrary access
 *            patterns
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static storage for memoized terms. */
static seq_t *cache = NULL;        /* array of found m values */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* allocated capacity                      */
static idx_t next_candidate = 0;   /* smallest m not yet tested              */

/* Grow the cache to hold at least `needed` entries. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();              /* fatal in the unlikely event of OOM */
    cache     = p;
    cache_cap = new_cap;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined offset, handled gracefully */

    size_t target = (size_t)n;                 /* `n` is 1‑based; cache index = n‑1 */

    /* Extend the cache until it contains the requested term. */
    if (cache_len < target) {
        grow_cache(target);
        while (cache_len < target) {
            unsigned long long val = 210ULL * (unsigned long long)next_candidate + 17ULL;
            if (is_prime(val))
                cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
