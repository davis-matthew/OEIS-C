/*
 * A190958  a(n) = 2*a(n-1) - 10*a(n-2), with a(0) = 0, a(1) = 1.
 * https://oeis.org/A190958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 10*a(n-2), with a(0)=0, a(1)=1
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 38, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    } else {
        /* Expand cache if needed */
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;         /* reallocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute values up to n */
    while (cache_size <= n) {
        seq_t val = 2 * cache[cache_size - 1] - 10 * cache[cache_size - 2];
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[(size_t)n];
}
