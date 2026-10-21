/*
 * A060405  Sum of n-th Lucas number (A000032(n)) and n-th Pell number
 *          (A000129(n)).
 * https://oeis.org/A060405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000129(n) + A000032(n) Recurrence (n >= 4): a(0) = 2,
 *            a(1) = 2, a(2) = 5, a(3) = 9 a(n) = 3*a(n-1) - 3*a(n-3) - a(n-4)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed a(i) values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    /* initialise cache with capacity 4 and the first four terms */
    if (cache == NULL) {
        cache_cap = 8;  /* arbitrary start capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory: return 0 */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)9;
        cache_len = 4;
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_len) {
        /* ensure capacity */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t term;
            if (i < 4) {          /* initial values are already set */
                term = cache[i];
            } else {
                term = 3 * cache[i-1];
                term -= 3 * cache[i-3];
                term -= cache[i-4];
            }
            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
