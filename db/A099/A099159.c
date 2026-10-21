/*
 * A099159  a(n) = (L(n-2)+2*3^n)/5.
 * https://oeis.org/A099159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (L(n-2) + 2*3^n) / 5 Recurrence: a(0)=1, a(1)=1, a(2)=4
 *            a(n) = 4*a(n-1) - 2*a(n-2) - 3*a(n-3) for n >= 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The program stores previously computed values in a dynamically
   resizable cache so that calls in arbitrary order run in O(n) time
   overall and use only integer arithmetic. */

#include <stdlib.h>

/* Static cache used by both ensure_cache and a(). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Create/extend the cache so that it contains values up to index n. */
static void ensure_cache(idx_t n)
{
    /* Lazy initialization: the first three values are known. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return;            /* allocation failed – caller will get 0 */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)1;           /* a(1) */
        cache[2] = (seq_t)4;           /* a(2) */
        cache_len = 3;
    }

    /* Grow only if the requested index is outside the current range. */
    if (n + 1 <= cache_len) return;

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;                /* allocation failed – keep old cache */
    cache = tmp;

    /* Compute the new terms using the recurrence. */
    for (idx_t i = cache_len; i < new_len; ++i) {
        cache[i] = 4 * cache[i - 1] - 2 * cache[i - 2] - 3 * cache[i - 3];
    }
    cache_len = new_len;
}

/* Public entry point that returns a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);           /* guarantees cache[n] exists */
    return cache ? cache[n] : 0;
}
