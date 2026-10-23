/*
 * A117761  Expansion of x*(1 + x^2 + x^4)/(1 - x - x^3 - x^5 - x^7).
 * https://oeis.org/A117761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 95, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7)
   initial values: 
   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=12
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* initialise cache with the 8 known initial values */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)8;
        cache[7] = (seq_t)12;
        cache_size = 8;
    }

    /* grow allocation if needed */
    if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n) + 1 > new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    if ((size_t)(n) + 1 > cache_size) {
        size_t start = cache_size;
        for (size_t i = start; i <= (size_t)(n); ++i) {
            seq_t val = 0;
            if (i >= 1) val += cache[i - 1];
            if (i >= 3) val += cache[i - 3];
            if (i >= 5) val += cache[i - 5];
            if (i >= 7) val += cache[i - 7];
            cache[i] = val;
        }
        cache_size = (size_t)(n) + 1;
    }

    return cache[(size_t)(n)];
}
