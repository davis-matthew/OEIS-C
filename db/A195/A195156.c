/*
 * A195156  a(n) = (16^n-1)/3.
 * https://oeis.org/A195156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^n - 1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(n) = 16 * a(n-1) + 5  for n > 0 */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;

/* Ensure that seq_cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* Initialise cache first element if it does not exist */
    if (seq_cache_len == 0) {
        seq_cache_len = 1;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cache_len);
        if (seq_cache == NULL) return;          /* allocation failed – nothing we can do */
        seq_cache[0] = 0;
        return;
    }

    /* If the cache already covers n, nothing to do */
    if (n < seq_cache_len) return;

    /* Allocate/expand the cache to hold indices up to n inclusive */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return;                    /* reallocation failed – keep old cache */
    seq_cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = seq_cache_len; i <= n; ++i) {
        seq_cache[i] = 16 * seq_cache[i-1] + 5;
    }

    seq_cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
