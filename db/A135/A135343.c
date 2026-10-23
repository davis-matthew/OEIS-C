/*
 * A135343  a(n) = 3*a(n-1) + 4*a(n-2) - a(n-3) + 3*a(n-4) + 4*a(n-5).
 * https://oeis.org/A135343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - a(n-3) + 3*a(n-4) + 4*a(n-5) with
 *            a(0)=1, a(1)=3, a(2)=12, a(3)=51, a(4)=205
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initial base values */
    static const seq_t base[5] = {(seq_t)1, (seq_t)3, (seq_t)12,
                                  (seq_t)51, (seq_t)205};

    /* --- initialize on first call ------------------------------------- */
    if (cache_len == 0) {
        /* start with a capacity of 5 to hold the base terms */
        cache_cap = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* fallback on allocation failure */
        for (idx_t i = 0; i < 5; ++i) cache[i] = base[i];
        cache_len = 5;
    }

    /* Return a cached value if already known */
    if (n < cache_len) return cache[n];

    /* --- extend capacity if necessary --------------------------------- */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 5;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        } else {
            /* on allocation failure return 0 as a safe fallback */
            return (seq_t)0;
        }
    }

    /* --- compute missing terms ---------------------------------------- */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)3 * cache[i - 1];
        val += (seq_t)4 * cache[i - 2];
        val -= cache[i - 3];
        val += (seq_t)3 * cache[i - 4];
        val += (seq_t)4 * cache[i - 5];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
