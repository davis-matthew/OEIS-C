/*
 * A086903  a(n) = 8*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 8.
 * https://oeis.org/A086903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - a(n-2), a(0) = 2, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated size in terms */
    static idx_t last = (idx_t)-1;/* last computed index */

    /* initialise cache with the two seed values */
    if (last == (idx_t)-1) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)8;
        last = 1;
        if (n <= 1) return cache[n];
    }

    /* if the requested index is already cached, return it */
    if (n <= last) return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc must succeed; if it doesn't, just use the old pointer */
        if (tmp) {
            cache = tmp;
            cap = new_cap;
        }
    }

    /* compute new terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)2;
        else if (i == 1)
            val = (seq_t)8;
        else
            val = cache[i - 1] * (seq_t)8 - cache[i - 2];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
