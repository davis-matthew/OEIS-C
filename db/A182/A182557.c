/*
 * A182557  a(n) = a(n-1)*2 + floor(sqrt(a(n-2))).
 * https://oeis.org/A182557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182557:
   a(0) = 0, a(1) = 1,
   a(n) = a(n-1)*2 + floor_sqrt(a(n-2))   for n ≥ 2 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;    /* number of computed elements (0..len-1 are valid) */
static size_t cache_cap = 0;    /* allocated capacity */

/* ensure cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache_cap = needed > 16 ? needed : 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        cache_len = 0;
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    ensure_capacity(n);

    /* initialise base cases if not done yet */
    if (cache_len < 2) {
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t t = cache[k-1] * (seq_t)2 + floor_sqrt(cache[k-2]);
        cache[k] = t;
    }
    cache_len = n + 1;

    return cache[n];
}
