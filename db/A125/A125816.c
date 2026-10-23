/*
 * A125816  a(n) = ((1+sqrt(13))^n + (1-sqrt(13))^n)/2.
 * https://oeis.org/A125816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) with a(0)=a(1)=1. The function
 *            computes a(n) where a(1) is the first OEIS term
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for the OEIS offset, but safe */
    size_t idx = (size_t)(n - 1);   /* convert to 0‑based index */

    if (cache_len == 0) {
        grow_cache(2);
        cache[0] = 1;             /* a(0) in recurrence == OEIS a(1) */
        cache[1] = 1;             /* a(1) in recurrence == OEIS a(2) */
        cache_len = 2;
    }

    if (idx >= cache_len) {
        grow_cache(idx + 1);
        while (cache_len <= idx) {
            seq_t val = 2 * cache[cache_len - 1] + 12 * cache[cache_len - 2];
            cache[cache_len] = val;
            cache_len++;
        }
    }

    return cache[idx];
}
