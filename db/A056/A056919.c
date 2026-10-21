/*
 * A056919  Numerators of continued fraction for left factorial.
 * https://oeis.org/A056919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = a(n-1) - floor(n/2)*a(n-2), with a(0)=0, a(1)=1
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t  cache_len = 0;  /* number of elements currently stored (last index + 1) */

seq_t a(idx_t n) {
    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        /* first call – allocate space for at least n+1 elements, but also for a(0) and a(1) */
        idx_t init_size = n + 1;
        if (init_size < 2) init_size = 2;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return 0;          /* fallback – out of memory */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = init_size;
        if (cache_len > 2) {
            /* nothing else to initialise – values beyond a(1) will be computed later */
        }
    } else if (n >= cache_len) {
        /* grow cache to hold up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* fallback – out of memory */
        cache = tmp;
        cache_len = new_size;
    }

    /* compute any missing values up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        /* use a wider signed intermediate to avoid overflow in 32‑bit builds */
        signed __int128 half = (signed __int128)(i / 2);
        signed __int128 term = (signed __int128)prev1 - half * (signed __int128)prev2;
        cache[i] = (seq_t)term;
    }

    return cache[n];
}
