/*
 * A183978  1/4 the number of (n+1) X 2 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4), with initial values
 *            a(1)=4, a(2)=6, a(3)=9, a(4)=15
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;
/* Number of elements currently in cache (always >= 0).  */
static idx_t cache_size = 0;

/* Extend the cache to contain values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – keep the old cache unchanged.  */
        return;
    }
    cache = tmp;

    /* Initialize the new entries.  */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0)          cache[i] = (seq_t)4;   /* a(1) */
        else if (i == 1)     cache[i] = (seq_t)6;   /* a(2) */
        else if (i == 2)     cache[i] = (seq_t)9;   /* a(3) */
        else if (i == 3)     cache[i] = (seq_t)15;  /* a(4) */
        else {
            /* Use the recurrence: a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4).  */
            seq_t v1 = cache[i - 1];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            seq_t val = (seq_t)3 * v1 + (seq_t)4 * v4;
            val -= (seq_t)6 * v3;
            cache[i] = val;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined region – return 0.  */
    ensure_cache(n);
    return cache[n - 1];
}
