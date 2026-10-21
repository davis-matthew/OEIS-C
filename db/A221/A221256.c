/*
 * A221256  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, with no occupancy
 *          greater than 2.
 * https://oeis.org/A221256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2) - 42*a(n-3) + 42*a(n-5) - 6*a(n-6) -
 *            6*a(n-7) + a(n-8) Initial values: a(1)=0, a(2)=4, a(3)=8,
 *            a(4)=81, a(5)=392, a(6)=2500, a(7)=14112, a(8)=83521
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal cache */
static seq_t *cache      = NULL;
static idx_t  cache_cap  = 0;   /* allocated entries, indices 0..cache_cap-1 */
static idx_t  last_computed = 0;/* largest index whose value is valid */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_cap) return;

    idx_t needed = n + 1;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return; /* out of memory – caller will not use the result */

#ifdef __cplusplus
    /* Default‑construct all new elements (C++ case). */
    for (idx_t i = 0; i < new_cap; ++i)
        new(&new_cache[i]) seq_t();
#endif

    /* Copy existing values */
    for (idx_t i = 0; i < cache_cap; ++i) {
#ifdef __cplusplus
        new_cache[i] = cache[i];
#else
        new_cache[i] = cache[i];
#endif
    }

    /* Properly destroy old objects (C++ case). */
#ifdef __cplusplus
    for (idx_t i = 0; i < cache_cap; ++i)
        cache[i].~seq_t();
#endif

    free(cache);
    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    static const seq_t init[9] = {
        0,      /* dummy for index 0 */
        0,      /* a(1) */
        4,      /* a(2) */
        8,      /* a(3) */
        81,     /* a(4) */
        392,    /* a(5) */
        2500,   /* a(6) */
        14112,  /* a(7) */
        83521   /* a(8) */
    };

    if (n == 0) return 0;        /* offset handling – value unused */
    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            /* Compute all positive terms first */
            seq_t pos = cache[i-1] * 6 + cache[i-2] * 6
                        + cache[i-5] * 42 + cache[i-8];
            /* Subtract the negative part */
            seq_t neg = cache[i-3] * 42 + cache[i-6] * 6
                        + cache[i-7] * 6;
            cache[i] = pos - neg;
        }
    }
    last_computed = n;
    return cache[n];
}
