/*
 * A123893  Expansion of g.f.: (1+x^2)*(1+2*x^2)*(1+3*x^2)/(1-4*x+6*x^2-18*x^3
 *          +11*x^4-22*x^5+6*x^6-6*x^7).
 * https://oeis.org/A123893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=16, a(3)=58, a(4)=208, a(5)=750, a(6)=2708;
 *            a(n)=4*a(n-1)-6*a(n-2)+18*a(n-3)-11*a(n-4)+22*a(n-5)-6*a(n-6)+6*a(n-7)
 *            for n ≥ 7. Offset: 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] contains a(i) for i < len */
    static seq_t *cache   = NULL;
    static size_t len     = 0;   /* number of values stored in cache */
    static size_t cap     = 0;   /* capacity of the cache array */

    /* helper to ensure cache has at least "need" slots */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < cap) new_cap = cap + 1;   /* avoid overflow */
        while (new_cap < (size_t)(n + 1)) {
            if (new_cap > SIZE_MAX / 2) break;
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed; return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first 7 terms on first use */
    if (len == 0) {
        /* ensure capacity of at least 7 */
        if (cap < 7) {
            size_t new_cap = 7;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cap   = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)58;
        cache[4] = (seq_t)208;
        cache[5] = (seq_t)750;
        cache[6] = (seq_t)2708;
        len = 7;
    }

    /* if requested index already computed, return it */
    if ((size_t)n < len) return cache[(size_t)n];

    /* compute missing terms up to n */
    size_t idx = (size_t)n;
    for (size_t i = len; i <= idx; ++i) {
        /* compute using a sum of positives minus a sum of negatives
           to avoid unsigned underflow */
        seq_t pos = 4 * cache[i - 1]
                  + 18 * cache[i - 3]
                  + 22 * cache[i - 5]
                  + 6 * cache[i - 7];
        seq_t neg = 6 * cache[i - 2]
                  + 11 * cache[i - 4]
                  + 6 * cache[i - 6];
        cache[i] = pos - neg;   /* pos >= neg for all n >= 7 */
    }

    if (idx + 1 > len) len = idx + 1;
    return cache[idx];
}
