/*
 * A122167  Expansion of x*(-1+5*x-6*x^2+x^3) / ( (2*x-1)*(x^3-3*x^2+1) ).
 * https://oeis.org/A122167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 33, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A122167: Expansion of x*(-1+5*x-6*x^2+x^3) /
 *                ((2*x-1)*(x^3-3*x^2+1))
 *
 * The generating function satisfies the recurrence
 *   a(n) = 2*a(n-1) + 3*a(n-2) - 7*a(n-3) + 2*a(n-4)
 * with initial values
 *   a(1)= 1,  a(2)=-3,  a(3)= 3,  a(4)=-11
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity (elements 0 .. cap-1) */
    static size_t computed = 0;     /* highest index with value stored (>=0) */

    /* ensure the requested index is within the allocated capacity */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap <= (size_t)n) newcap *= 2;

        seq_t *new_cache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure: return 0 sentinel */

        /* copy existing data if any, initialise zero for new elements */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        memset(&new_cache[cap], 0, (newcap - cap) * sizeof(seq_t));
        cache = new_cache;
        cap = newcap;

        /* initialise the dummy index 0 (unused) */
        cache[0] = 0;
    }

    /* compute values up to n if they are not yet computed */
    if ((size_t)n > computed) {
        size_t start = computed + 1;
        if (start < 1) start = 1;

        for (size_t i = start; i <= (size_t)n; ++i) {
            if (i == 1) cache[1] =  1;
            else if (i == 2) cache[2] = -3;
            else if (i == 3) cache[3] =  3;
            else if (i == 4) cache[4] = -11;
            else
                cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2]
                           - 7 * cache[i - 3] + 2 * cache[i - 4];
        }
        computed = (size_t)n;
    }

    return cache[n];
}
