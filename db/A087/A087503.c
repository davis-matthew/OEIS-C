/*
 * A087503  a(n) = 3*(a(n-2) + 1), with a(0) = 1, a(1) = 3.
 * https://oeis.org/A087503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 3 * (a(n-2) + 1) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 79, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[n] holds a(n) */
static idx_t cache_size = 0;  /* current size of cache (number of elements stored) */

/* Ensure that cache has room for values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;   /* already enough */

    /* Resize: preserve existing values */
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc((void *)cache, (n + 1) * sizeof(seq_t));
    }
    if (new_cache == NULL) {
        /* Allocation failed; for the purposes of this implementation we
           simply abort since sequence values cannot be computed. */
        abort();
    }
    cache = new_cache;

    /* Initialize new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else {
            /* a(i) = 3 * (a(i-2) + 1) */
            seq_t tmp = cache[i - 2] + (seq_t)1;
            cache[i] = tmp * (seq_t)3;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
