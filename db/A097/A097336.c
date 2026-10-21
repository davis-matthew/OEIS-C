/*
 * A097336  Sum k=0..n, C(n-k, floor(k/2))4^k.
 * https://oeis.org/A097336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0)=1, a(1)=5, a(2)=5,
 * a(n)=a(n-1)+16*a(n-3)  (n≥3)
 * This implements OEIS A097336.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* cached values */
static size_t cache_len = 0;          /* number of cached elements */

/* Expand the cache up to index n, computing new entries by the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return;                      /* already cached */

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failed */
    cache = tmp;

    /* fill new entries starting from the current length */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0)                cache[i] = (seq_t)1;
        else if (i == 1)           cache[i] = (seq_t)5;
        else if (i == 2)           cache[i] = (seq_t)5;
        else                       cache[i] = cache[i-1] + (seq_t)16 * cache[i-3];
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
