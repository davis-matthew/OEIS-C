/*
 * A208689  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 3*a(n-3); initial values: a(1)=6,
 *            a(2)=36, a(3)=78
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static cache for memoised values */
static seq_t *cache = NULL;    /* array indexed from 1 (index 0 unused)   */
static idx_t  cap   = 0;        /* allocated capacity of cache array      */
static idx_t  max_c = 0;        /* highest n already computed (≥0)        */

/* Ensure that cache holds values up to at least n */
static void keep_cache(idx_t n)
{
    if (n <= max_c) return;                /* nothing to do          */

    /* enlarge cache if necessary */
    idx_t need = n + 1;                    /* +1 because we ignore index 0   */
    if (cap < need) {
        idx_t new_cap = (cap > 0) ? cap : 16;
        while (new_cap < need) new_cap <<= 1;   /* double until big enough     */
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(tmp, cache, cap * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = max_c + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)6;
        } else if (i == 2) {
            cache[i] = (seq_t)36;
        } else if (i == 3) {
            cache[i] = (seq_t)78;
        } else {
            /* recurrence: a(i) = 2*a(i-1) + 4*a(i-2) - 3*a(i-3) */
            cache[i] = (seq_t)(2 * cache[i-1] + 4 * cache[i-2] - 3 * cache[i-3]);
        }
    }
    max_c = n;
}

/* Public entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for zero or negative */

    keep_cache(n);
    return cache[n];
}
