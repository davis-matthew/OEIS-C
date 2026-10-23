/*
 * A110689  Expansion of
 *          (2*x+1)*(4*x^2+8*x+1)/((3*x^2+3*x+1)*(2*x^3+2*x^2+4*x+1)).
 * https://oeis.org/A110689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (2*x+1)*(4*x^2+8*x+1) /*
 *            /((3*x^2+3*x+1)*(2*x^3+2*x^2+4*x+1))
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 34, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n >= 5
 *   a(n) = -(7*a(n-1) + 17*a(n-2) + 20*a(n-3) + 12*a(n-4) + 6*a(n-5))
 * Initial values: a(0)=1, a(1)=3, a(2)=-18, a(3)=63, a(4)=-207
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Helper to ensure cache can hold up to index 'max_idx' */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!new_cache) return 0; /* allocation failed */
        } else {
            new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!new_cache) {
                /* realloc failed, keep old cache to avoid leaks */
                new_cache = cache;
                new_len = cache_len; /* keep old size */
            }
        }
        cache = new_cache;
        /* Fill new entries up to new_len - 1 */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) cache[0] = (seq_t)1;
            else if (i == 1) cache[1] = (seq_t)3;
            else if (i == 2) cache[2] = (seq_t)(-18);
            else if (i == 3) cache[3] = (seq_t)63;
            else if (i == 4) cache[4] = (seq_t)(-207);
            else /* i >= 5 */
                cache[i] = -( (seq_t)7 * cache[i-1]
                           + (seq_t)17 * cache[i-2]
                           + (seq_t)20 * cache[i-3]
                           + (seq_t)12 * cache[i-4]
                           + (seq_t)6 * cache[i-5] );
        }
        cache_len = new_len;
    }
    return cache[n];
}
