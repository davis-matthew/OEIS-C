/*
 * A054323  Fifth column of Lanczos triangle A053125 (decreasing powers).
 * https://oeis.org/A054323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n+5, 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative recurrence: a(n) = 20*a(n-1) - 160*a(n-2) + 640*a(n-3) - 1280*a(n-4) + 1024*a(n-5) */
/* with initial values a(0)=5, a(1)=140, a(2)=2016, a(3)=21120, a(4)=183040 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: 0..cache_size-1 already computed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Resize and fill cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (new_cache == NULL) {
            /* allocation failed; abort */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 5;
            } else if (i == 1) {
                cache[1] = 140;
            } else if (i == 2) {
                cache[2] = 2016;
            } else if (i == 3) {
                cache[3] = 21120;
            } else if (i == 4) {
                cache[4] = 183040;
            } else {
                /* recurrence: a(n) = 20*a(n-1) - 160*a(n-2) + 640*a(n-3) - 1280*a(n-4) + 1024*a(n-5) */
                seq_t term = 20 * cache[i-1];
                term -= 160 * cache[i-2];
                term += 640 * cache[i-3];
                term -= 1280 * cache[i-4];
                term += 1024 * cache[i-5];
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
