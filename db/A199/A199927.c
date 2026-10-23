/*
 * A199927  Trisection 0 of A199802.
 * https://oeis.org/A199927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199927:
   Generating function: (1+2*x-x^2)/(1+x+5*x^2-x^3+x^4)
   Recurrence for n >= 3:
      a(n) = -a(n-1) - 5*a(n-2) + a(n-3) - a(n-4)
   Initial values:
      a(0)=1, a(1)=1, a(2)=-7, a(3)=3
   This implementation uses memoisation to allow arbitrary-order
   queries, allocating space as needed. */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms currently stored */
    static size_t cache_cap  = 0;  /* allocated capacity */

    /* Initialise cache with the four known terms */
    if (!cache) {
        cache_cap  = 16;
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)1;
        cache[2]   = (seq_t)(-7);
        cache[3]   = (seq_t)3;
        cache_size = 4;
    }

    /* If the requested index is already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms from the current size up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= 4) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            cache[i] = -a1 - (seq_t)5 * a2 + a3 - a4;
        }
        /* indices 0..3 are already filled during initialisation */
    }

    cache_size = n + 1;
    return cache[n];
}
