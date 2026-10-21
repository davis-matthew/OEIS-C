/*
 * A228204  y-values in the solution to x^2 - 13y^2 = 27.
 * https://oeis.org/A228204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1298*a(n-4) - a(n-8) with initial values a(1)=3, a(2)=11,
 *            a(3)=61, a(4)=213, a(5)=4107, a(6)=14339, a(7)=79189,
 *            a(8)=276477 (offset = 1). Computation uses memoisation and runs
 *            in O(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache of computed terms   */
static size_t cache_cap = 0;         /* number of allocated slots (size-1)*/
static size_t cache_max = 0;         /* highest index that has been computed */

static const seq_t init_vals[8] = {
    (seq_t)3,   (seq_t)11,   (seq_t)61,   (seq_t)213,
    (seq_t)4107, (seq_t)14339, (seq_t)79189, (seq_t)276477
};

static void ensure_cache(idx_t needed) {
    size_t need = (size_t) needed;
    if (need <= cache_cap) return;

#ifdef __cplusplus
    seq_t *newp = new seq_t[need + 1];
    if (cache) {
        for (size_t i = 0; i <= cache_cap; ++i)
            newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
#else
    seq_t *newp = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
    cache = newp;
#endif
    cache_cap = need;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1 */

    if (n <= (idx_t)8) return init_vals[n - 1];

    ensure_cache(n);

    if (cache_max < (size_t)8) {
        for (size_t i = 1; i <= (size_t)8; ++i)
            cache[i] = init_vals[i - 1];
        cache_max = 8;
    }

    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)1298 * cache[i - 4] - cache[i - 8];
    }
    cache_max = (size_t)n;

    return cache[(size_t)n];
}
