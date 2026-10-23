/*
 * A175715  Expansion of 1/(1 - x - x^2 - 3*x^4 + 4*x^5 - 2*x^6).
 * https://oeis.org/A175715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 3*a(n-4) - 4*a(n-5) + 2*a(n-6)
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=8, a(5)=10 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache that remembers computed terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t last_computed = 0;   /* largest index whose value is stored */
    static bool initialized = false;

    /* First call: set up cache with the initial six values. */
    if (!initialized) {
        capacity = 64;                 /* start with a reasonable capacity */
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)10;

        last_computed = 5;
        initialized = true;
    }

    /* Grow the cache array if necessary. */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n. */
    if ((size_t)n > last_computed) {
        for (idx_t i = (idx_t)(last_computed + 1); i <= n; ++i) {
            /* For i < 6 the recurrence is not needed; values are already
               in the cache. The loop starts with i = 6, where all indices
               accessed below are valid. */
            seq_t term =
                cache[i - 1] +                        /* a(n-1) */
                cache[i - 2] +                        /* a(n-2) */
                3 * cache[i - 4];                     /* + 3*a(n-4) */
            term -= 4 * cache[i - 5];                 /* - 4*a(n-5) */
            term += 2 * cache[i - 6];                 /* + 2*a(n-6) */
            cache[i] = term;
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
