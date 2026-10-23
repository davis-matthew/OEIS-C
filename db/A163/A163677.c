/*
 * A163677  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*a(n-1)+39*a(n-2)+39*a(n-3)+39*a(n-4)-780*a(n-5) with
 *            a(0)=1, a(1)=41, a(2)=1640, a(3)=65600, a(4)=2624000 (a(5)
 *            additionally +1 due to the numerator of the G.f.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t* seq_alloc(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void seq_free(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n) {
    /* static cache for dynamic programming */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed_max = 0;
    static bool initialized = false;

    /* ensure cache exists and has base values */
    if (!initialized) {
        size_t initcap = 16;
        while ((size_t)n >= initcap) initcap <<= 1;
        cache = seq_alloc(initcap);
        for (size_t i = 0; i < initcap; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;
        if (initcap > 1) cache[1] = (seq_t)41;
        if (initcap > 2) cache[2] = (seq_t)1640;
        if (initcap > 3) cache[3] = (seq_t)65600;
        if (initcap > 4) cache[4] = (seq_t)2624000;
        cap = initcap;
        computed_max = 4;
        initialized = true;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t* newcache = seq_alloc(newcap);
        for (size_t i = 0; i < cap; ++i) newcache[i] = cache[i];
        for (size_t i = cap; i < newcap; ++i) newcache[i] = (seq_t)0;
        seq_free(cache);
        cache = newcache;
        cap = newcap;
    }

    /* if already computed, return */
    if (n <= computed_max) {
        return cache[n];
    }

    /* constants */
    const seq_t C39 = (seq_t)39;
    const seq_t C780 = (seq_t)780;
    const seq_t ONE = (seq_t)1;

    /* compute new terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 5) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            cache[i] = sum * C39 - cache[0] * C780 + ONE;
        } else {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            cache[i] = sum * C39 - cache[i-5] * C780;
        }
    }
    computed_max = n;
    return cache[n];
}
