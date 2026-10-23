/*
 * A131216  Numbers X such that 99*X^2 - 2178 is a square.
 * https://oeis.org/A131216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 20*a(n+1) - a(n), with a(1)=11, a(2)=209
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n), n >= 1 */
seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* allocated capacity */

    /* ensure enough space in cache */
    if (len < (size_t)n) {                 /* need to compute more terms */
        size_t new_cap = cap ? cap : 2;    /* start with small capacity */
        while (new_cap < (size_t)n) {
            new_cap *= 2;                  /* double until big enough */
        }
        /* reallocate (grow or create) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate */
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = len; i < (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)11;      /* a(1) */
        } else if (i == 1) {
            cache[i] = (seq_t)209;     /* a(2) */
        } else {
            /* recurrence: a(k) = 20*a(k-1) - a(k-2)  for k = i+1 >= 3 */
            cache[i] = 20 * cache[i - 1] - cache[i - 2];
        }
    }
    len = n;   /* update stored length */

    return cache[(size_t)n - 1];
}
