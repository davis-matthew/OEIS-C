/*
 * A158784  Expansion of (1+4*x+14*x^2)/(1-2*x^2-64*x^3).
 * https://oeis.org/A158784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A158784
    Formula: a(n) = 2*a(n-2) + 64*a(n-3),  a(0)=1, a(1)=4, a(2)=16
    (Computed directly from the linear recurrence, no exponential steps.) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache to allow arbitrary order calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t size = 0;  /* number of cached values */

    /* One‑time initialization */
    if (cache == NULL) {
        cap = 16;                               /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* safety */
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)4;
        if (cap > 2) cache[2] = (seq_t)16;
        size = 3;                               /* we have values 0..2 */
    }

    /* If the request is already cached, return it */
    if ((idx_t)n < size)
        return cache[(size_t)n];

    /* Ensure we have enough capacity */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – unlikely for test data */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = size; i < needed; ++i) {
        seq_t val = (seq_t)2 * cache[i - 2] + (seq_t)64 * cache[i - 3];
        cache[i] = val;
    }
    size = needed;
    return cache[(size_t)n];
}
