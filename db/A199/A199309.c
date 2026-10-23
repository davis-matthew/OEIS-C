/*
 * A199309  a(n) = (7*5^n+1)/2.
 * https://oeis.org/A199309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*5^n + 1) / 2 (OEIS A199309) * Recurrence: a(0) = 4;
 *            a(n) = 5*a(n-1) - 2 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, grows as needed */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* current allocated capacity */
    static idx_t  size = 0;  /* number of values stored (next free index) */

    /* initialise cache to hold a(0) */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0; /* allocation failure fallback */
        cache[0] = (seq_t)4;           /* a(0) */
        size = 1;
    }

    /* if requested value already computed, return it */
    if (n < size)
        return cache[n];

    /* grow cache until we can store a(n) */
    while (size <= n) {
        /* double the capacity when needed */
        if (size >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0; /* allocation failure fallback */
            cache = tmp;
            cap = new_cap;
        }

        /* compute next value using the recurrence */
        cache[size] = cache[size - 1] * (seq_t)5 - (seq_t)2;
        ++size;
    }

    return cache[n];
}
