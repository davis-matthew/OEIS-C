/*
 * A033970  Trajectory of 1 under map n->27n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 24, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, a(n) = a(n-1)/2 if a(n-1) even else 27*a(n-1)+1 */
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t  cache_len = 0;     /* number of initialized terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure the cache holds indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                seq_t prev = cache[i-1];
                if ((prev & 1) == 0)
                    cache[i] = prev >> 1;
                else
                    cache[i] = prev * 27 + 1;
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
