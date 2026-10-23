/*
 * A017889  Expansion of 1/(1-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10)+a(n-11)+a(n-12)+a(n-13) Initial values: a(0)=1,
 *            a(1..9)=0, a(10)=a(11)=a(12)=a(13)=1
 * Range:     u8 n <= 65, u16 n <= 110, u32 n <= 202, u64 n <= 385,
 *            u128 n <= 751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* size allocated for cache */
    static idx_t computed = 0;     /* highest index already computed */

    /* initial values for indices 0..13 */
    const seq_t init_values[14] = {
        (seq_t)1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1
    };

    /* initialise cache on first call or enlarge it if needed */
    if (cache == NULL) {
        idx_t size = (n >= 14) ? (n + 1) : 14;
        cache = (seq_t*)malloc((size_t)size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – unlikely */
        cache_size = size;
        for (idx_t i = 0; i < 14; ++i) cache[i] = init_values[i];
        computed = 13;
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        cache = tmp;
        /* initialise any new indices up to new_size, if below 14 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 14) cache[i] = init_values[i];
        }
        cache_size = new_size;
        /* 'computed' remains unchanged; will be filled below */
    }

    /* compute any missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i < 14) {
                cache[i] = init_values[i];
            } else {
                cache[i] = cache[i-10] + cache[i-11] + cache[i-12] + cache[i-13];
            }
        }
        computed = n;
    }

    return cache[n];
}
