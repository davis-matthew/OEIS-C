/*
 * A033973  Trajectory of 1 under map n->35n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n+1) = (a(n) % 2 ? 35 * a(n) + 1 : a(n) / 2) */
#include <stdlib.h>

static seq_t *cache = NULL;    /* cached values of the sequence */
static size_t cache_size = 0;  /* number of cached elements (last index + 1) */
static size_t cache_cap  = 0;  /* allocated capacity of cache */

static void ensure_cache(idx_t n)
{
    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }

    /* Grow the cache until it holds n + 1 elements */
    while (cache_cap <= (size_t)n) {
        cache_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
    }

    /* Compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 1;       /* a(0) */
        } else {
            seq_t prev = cache[i - 1];
            if (prev & 1) {          /* odd */
                cache[i] = prev * (seq_t)35 + (seq_t)1;
            } else {                 /* even */
                cache[i] = prev >> 1;
            }
        }
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
