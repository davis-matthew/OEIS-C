/*
 * A102002  Weighted tribonacci (1,2,4), companion to A102001.
 * https://oeis.org/A102002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 4*a(n-3), a(1)=1, a(2)=7, a(3)=13
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused; cache[i] = a(i) */
static idx_t   cache_size = 0;       /* highest index currently computed */

/* Ensure the cache can provide values up to `n`.  If necessary, grow it. */
static void ensure_cache(idx_t n) {
    if (n < 1) return;
    if (n < cache_size) return;        /* already have them */
    idx_t new_size = n + 1;             /* indices 0..n inclusive */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
    } else {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    }
    /* Initialise any newly allocated entries that are not yet computed */
    if (cache == NULL) {
        /* Allocation failed; abort (undefined behaviour for this task) */
        exit(1);
    }
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else if (i == 3) {
            cache[i] = (seq_t)13;
        } else if (i > 3) {
            /* Compute using the recurrence */
            cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] + (seq_t)4 * cache[i-3];
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined offset; return 0 as placeholder */
    ensure_cache(n);
    return cache[n];
}
