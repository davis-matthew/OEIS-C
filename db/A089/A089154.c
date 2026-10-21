/*
 * A089154  Expansion of 2*x*(x+2) / ((x-1)*(x^2+6*x-1)).
 * https://oeis.org/A089154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) - a(n-3); a(1)=4, a(2)=30, a(3)=190
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_size = n;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp && new_size > 0) {
        /* Allocation failed; in a test environment this should not happen. */
        return;
    }
    cache = tmp;

    /* Initialise small indices if missing. */
    if (cache_size == 0) {
        cache[0] = (seq_t)4;
        if (new_size >= 2) cache[1] = (seq_t)30;
        if (new_size >= 3) cache[2] = (seq_t)190;
    } else {
        if (cache_size < 1 && new_size >= 1) cache[0] = (seq_t)4;
        if (cache_size < 2 && new_size >= 2) cache[1] = (seq_t)30;
        if (cache_size < 3 && new_size >= 3) cache[2] = (seq_t)190;
    }

    /* Compute any new terms using the recurrence. */
    idx_t start = cache_size < 3 ? 3 : cache_size;
    for (idx_t i = start; i < new_size; ++i) {
        cache[i] = (seq_t)7 * cache[i-1];
        cache[i] -= (seq_t)5 * cache[i-2];
        cache[i] -= cache[i-3];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n-1];
}
