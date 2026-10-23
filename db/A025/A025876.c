/*
 * A025876  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A025876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+a(n-6)+a(n-7)-a(n-11)-a(n-12)-a(n-13)+a(n-18);
 *            Initial values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0
 * Range:     u8 n <= 318, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises results so that any order of calls works and
   each new value is computed in O(1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Expand cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Compute values from the first missing index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i < 5) {
                cache[i] = 0;
            } else {
                seq_t val = 0;
                if (i >= 5) val += cache[i-5];
                if (i >= 6) val += cache[i-6];
                if (i >= 7) val += cache[i-7];
                if (i >= 11) val -= cache[i-11];
                if (i >= 12) val -= cache[i-12];
                if (i >= 13) val -= cache[i-13];
                if (i >= 18) val += cache[i-18];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
