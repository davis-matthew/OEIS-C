/*
 * A144745  Recurrence sequence a(n)=a(n-1)^2-a(n-1)-1, a(0)=9.
 * https://oeis.org/A144745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-1) - 1, a(0) = 9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure that the cache can hold up to index `needed` (inclusive). */
static void
ensure_cache(idx_t needed)
{
    if (needed + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function to evaluate the sequence term at index n. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first term if not done yet. */
    if (cache_len == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)9;
        cache_len = 1;
    }

    /* Extend cache up to index n, if needed. */
    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next = prev * prev - prev - (seq_t)1;
            cache[i] = next;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
