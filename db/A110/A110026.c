/*
 * A110026  Minimal number of times a rectangular grid of n X n+1 elements can
 *          be slid along a 45-degree line before a rotated version of the
 *          initial grid appears.
 * https://oeis.org/A110026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-2)*(2n-1)/gcd(a(n-2), 2n-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Euclidean algorithm for seq_t */
static inline seq_t mygcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++: use std::vector for automatic construction/destruction */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(2);
        cache[0] = (seq_t)1;    /* a(0) */
        cache[1] = (seq_t)1;    /* a(1) */
    }
    if ((idx_t)cache.size() <= n) {
        size_t old = cache.size();          /* first index to compute */
        cache.resize((size_t)n + 1);
        for (idx_t i = old; i <= n; ++i) {
            seq_t prev = cache[i - 2];
            seq_t mult = (seq_t)(2 * i - 1);
            seq_t g = mygcd(prev, mult);
            cache[i] = (prev / g) * mult;
        }
    }
    return cache[(size_t)n];
#else
    /* C: raw allocation, only works when seq_t is a POD integer type */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* highest index computed */

    if (cache == NULL) {
        /* allocate space for indices 0 and 1 */
        size_t init_size = 2;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;                 /* a(0) */
        cache[1] = (seq_t)1;                 /* a(1) */
        max_n = 1;
    }

    if ((idx_t)n > max_n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];
            seq_t mult = (seq_t)(2 * i - 1);
            seq_t g = mygcd(prev, mult);
            cache[i] = (prev / g) * mult;
        }
        max_n = n;
    }
    return cache[(size_t)n];
#endif
}
