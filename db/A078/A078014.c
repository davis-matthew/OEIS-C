/*
 * A078014  Expansion of (1-x)/(1-x+2*x^3).
 * https://oeis.org/A078014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=0,
   a(n)=a(n-1)-2*a(n-3)  for n>=3
   This is obtained from (1-x)/(1-x+2x^3). */
#include <stdlib.h>

/* You may include stdlib for malloc/realloc; no other headers needed. */

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* extend cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        /* realloc: new_len * sizeof(seq_t) bytes */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate as no graceful fallback can be provided */
            abort();
        }
        cache = tmp;
        /* initialize new entries */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1 || i == 2)
                cache[i] = (seq_t)0;
            else
                /* recurrence: a(i) = a(i-1) - 2*a(i-3) */
                cache[i] = cache[i - 1] - ((seq_t)2) * cache[i - 3];
        }
        cache_len = new_len;
    }
    return cache[n];
}
