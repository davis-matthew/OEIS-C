/*
 * A099488  Expansion of (1-x)^2/((1+x^2)*(1-4*x+x^2)).
 * https://oeis.org/A099488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) - a(n-4) (with initial
 *            values a(0)=1, a(1)=2, a(2)=7, a(3)=28)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* number of cached terms */

    /* ensure that cache holds terms up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = cache
            ? (seq_t*)realloc(cache, new_cap * sizeof(seq_t))
            : (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();     /* allocation failure */

        /* initialise the new entries */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i == 0) new_cache[i] = 1;
            else if (i == 1) new_cache[i] = 2;
            else if (i == 2) new_cache[i] = 7;
            else if (i == 3) new_cache[i] = 28;
            else {
                /* compute the recurrence in a way that avoids
                   underflow on unsigned types:                     */
                seq_t t1 = 4 * new_cache[i-1];
                seq_t t2 = 4 * new_cache[i-3];
                seq_t value = t1 + t2;
                value -= 2 * new_cache[i-2];
                value -= new_cache[i-4];
                new_cache[i] = value;
            }
        }
        /* update static cache pointer and capacity */
        cache = new_cache;
        cap   = new_cap;
    }

    /* return the required term */
    return cache[(size_t)n];
}
