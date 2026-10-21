/*
 * A218721  a(n) = (18^n-1)/17.
 * https://oeis.org/A218721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18^n-1)/17; a(0)=0, a(1)=1; recurrence
 *            a(n)=19*a(n-1)-18*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (number of elements) */
static idx_t  cache_len = 0;   /* number of entries actually filled */

seq_t a(idx_t n)
{
    /* allocate or grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* at least n+1 elements */
        if (new_cap < 2) new_cap = 2;            /* minimal growth for first calls */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases */
    if (cache_len == 0) {
        seq_cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        seq_cache[1] = 1;
        cache_len = 2;
    }

    /* return cached value if already computed */
    if (n < cache_len) {
        return seq_cache[n];
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_cache[i] = (seq_t)19 * seq_cache[i-1] - (seq_t)18 * seq_cache[i-2];
    }
    cache_len = n + 1;

    return seq_cache[n];
}
