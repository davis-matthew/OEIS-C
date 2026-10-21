/*
 * A094935  E.g.f.: exp(8x)/(1-8x)^(1/8).
 * https://oeis.org/A094935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n+1)*a(n-1) - 64*(n-1)*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t computed = 0;

    /* Ensure cache is large enough */
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        seq_t *newcache = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!newcache) return 0;          /* unlikely in test harness */
        for (idx_t i = 0; i < cap; ++i) newcache[i] = cache[i];
        free(cache);
        cache = newcache;
        cap = newcap;
    }

    /* Initialize first terms if not yet done */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }
    if (computed == 1 && n >= 1) {
        cache[1] = (seq_t)9;
        computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        idx_t m1 = 8 * i + 1;
        idx_t m2 = 64 * (i - 1);
        seq_t term1 = (seq_t)(m1) * prev1;
        seq_t term2 = (seq_t)(m2) * prev2;
        cache[i] = term1 - term2;
    }
    if (computed <= n) computed = n + 1;

    return cache[n];
}
