/*
 * A107433  Slowest increasing sequence where some digit of a(n) and some
 *          digit of a(n+1) add up to 9.
 * https://oeis.org/A107433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_sum9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107433: a(0) = 0;
   for n > 0, a(n) is the smallest integer greater than a(n‑1) such that
   some digit of a(n‑1) and some digit of a(n) sum to 9. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_sum9.h"

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static idx_t cacheSize = 0;   /* number of stored terms */
static idx_t cacheCap  = 0;   /* capacity of the cache */

static void ensure_cache(idx_t newSize)
{
    if (newSize <= cacheCap)
        return;

    size_t newCap = cacheCap ? cacheCap : 16;
    while (newCap < (size_t)newSize)
        newCap <<= 1;

    seq_t *newCache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (!newCache)   /* allocation failure – abort without modifying cache */
        return;

    cache = newCache;
    cacheCap = newCap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < cacheSize)
        return cache[n];

    ensure_cache(n + 1);

    for (idx_t i = cacheSize; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 0;
        } else {
            seq_t cur = cache[i - 1];
            seq_t cand = cur + (seq_t)1;
            while (!has_sum9(cur, cand))
                ++cand;
            value = cand;
        }
        cache[i] = value;
    }
    cacheSize = n + 1;
    return cache[n];
}
