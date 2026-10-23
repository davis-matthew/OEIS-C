/*
 * A130610  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+359)^2 = y^2.
 * https://oeis.org/A130610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 718 (for n > 6) Initial values:
 *            a(1)=0, a(2)=40, a(3)=901, a(4)=1077, a(5)=1281, a(6)=6160
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Static cache: 1‑based entries stored at cache[i-1] */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* allocated capacity (in 1‑based index) */
    static size_t computed_max = 0;   /* maximum n that has been computed */

    /* Handle the first six values directly. */
    if (n <= 6) {
        static const seq_t init[6] = {0, 40, 901, 1077, 1281, 6160};
        return init[(size_t)(n - 1)];
    }

    /* Allocate initial cache if needed. */
    if (!cache) {
        size_t init_cap = 64;  /* starting capacity */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory – return 0 as a safe failure */
#ifdef __cplusplus
        for (size_t i = 0; i < init_cap; ++i) new (&cache[i]) seq_t();
#endif
        /* Store the first six terms. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)901;
        cache[3] = (seq_t)1077;
        cache[4] = (seq_t)1281;
        cache[5] = (seq_t)6160;
        cache_len = init_cap;
        computed_max = 6;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_len;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* out‑of‑memory */
#ifdef __cplusplus
        for (size_t i = 0; i < new_cap; ++i) new (&new_cache[i]) seq_t();
        /* Copy existing values */
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
#else
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
#endif
        free(cache);
        cache = new_cache;
        cache_len = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 4] * (seq_t)6
                    - cache[i - 7] + (seq_t)718;
        cache[i - 1] = val;
    }
    computed_max = (size_t)n;

    return cache[n - 1];
}
