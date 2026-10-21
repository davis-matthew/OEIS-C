/*
 * A054761  Number of positive braids with n crossings of 5 strands.
 * https://oeis.org/A054761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 3*a(n-2) - 3*a(n-3) + 2*a(n-4) + 2*a(n-6) - a(n-10),
   a(0..9) = {1,4,13,37,99,254,636,1567,3822,9261}. */

#include <stdlib.h>

static const seq_t init[10] = {
    (seq_t)1,  (seq_t)4,  (seq_t)13, (seq_t)37,
    (seq_t)99, (seq_t)254, (seq_t)636, (seq_t)1567,
    (seq_t)3822, (seq_t)9261
};

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries already stored */

    /* Ensure the cache can hold indices 0..n. */
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        cache_len = 0;
    } else if (needed > cache_len) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    }

    /* Initialize any new slots that correspond to the first 10 terms. */
    idx_t i;
    for (i = cache_len; i < needed; ++i) {
        if (i < 10) {
            cache[i] = init[i];
            continue;
        }

        /* Recurrence for i >= 10. */
        seq_t term = cache[i-1] * (seq_t)4;
        term -= cache[i-2] * (seq_t)3;
        term -= cache[i-3] * (seq_t)3;
        term += cache[i-4] * (seq_t)2;
        term += cache[i-6] * (seq_t)2;
        term -= cache[i-10];
        cache[i] = term;
    }

    cache_len = needed;
    return cache[n];
}
