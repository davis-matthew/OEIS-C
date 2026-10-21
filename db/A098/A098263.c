/*
 * A098263  Chebyshev polynomials S(n,731).
 * https://oeis.org/A098263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 731*a(n-1) - a(n-2), a(0) = 1, a(1) = 731
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   mul_731
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_731.h"

/* Cached implementation of the recurrence
   a(n) = 731*a(n-1) - a(n-2)  (n >= 2)   */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed values      */
    static idx_t capacity = 0;       /* allocated size of cache      */
    static idx_t size = 0;           /* number of values stored       */

    /* ---------- initialise the cache on the first call ---------- */
    if (cache == NULL) {
        capacity = (n < 2 ? 2 : n + 1);   /* at least 2 for a(0), a(1) */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)                    /* out of memory      */
            return (seq_t)0;
        cache[0] = (seq_t)1;                  /* a(0) */
        cache[1] = (seq_t)731;                /* a(1) */
        size = 2;
    }
    /* ---------- grow the cache if the requested index is out of bounds ---------- */
    else if (n >= capacity) {
        idx_t new_cap = capacity;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                      /* keep old cache free */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* ---------- extend the cache up to index n if necessary ---------- */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t prevprev = cache[i - 2];
            cache[i] = mul_731(prev) - prevprev;
        }
        size = n + 1;
    }
    return cache[n];
}
