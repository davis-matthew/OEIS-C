/*
 * A135139  a(n) = 5*a(n-2) + 2*a(n-3).
 * https://oeis.org/A135139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) + 2*a(n-3); a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for memoisation */
static seq_t *cache = NULL;
static size_t cap = 0;

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n) {
    size_t required = (size_t)n + 1;
    if (required <= cap)
        return;                           /* already computed */

    if (cache == NULL) {                   /* first allocation */
        cache = (seq_t*)malloc(required * sizeof(seq_t));
    } else {                               /* grow existing array */
        cache = (seq_t*)realloc(cache, required * sizeof(seq_t));
    }

    /* Compute new entries */
    for (size_t i = cap; i < required; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else if (i == 1)
            cache[1] = (seq_t)2;
        else if (i == 2)
            cache[2] = (seq_t)4;
        else
            cache[i] = (seq_t)(5 * cache[i-2] + 2 * cache[i-3]);
    }
    cap = required;
}

/* Public function */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[(size_t)n];
}
