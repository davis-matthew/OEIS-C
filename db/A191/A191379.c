/*
 * A191379  Expansion of product(n>=0,
 *          1+2*x^(3^n)+x^(2*3^n)+2*x^(3*3^n)+x^(4*3^n) ).
 * https://oeis.org/A191379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A191379 (OEIS A191379):
   a(0) = 1
   For n > 0 let r = n % 3:
   if r == 0: a(n) = a(n/3) + 2*a((n-3)/3)
   if r == 1: a(n) = 2*a((n-1)/3) + a((n-4)/3)    (empty terms for negative arguments)
   if r == 2: a(n) = a((n-2)/3)  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {          /* initialise cache with a(0) = 1 */
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
    }

    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        for (idx_t i = old_len; i < cache_len; ++i) {
            seq_t val = 0;
            idx_t r = i % 3;
            if (r == 0) {
                /* a(i) = a(i/3) + 2*a((i-3)/3) if i>=3 */
                val = cache[i / 3];
                if (i >= 3) val += (seq_t)2 * cache[(i - 3) / 3];
            } else if (r == 1) {
                /* a(i) = 2*a((i-1)/3) + a((i-4)/3) if i>=4 */
                if (i >= 1) val += (seq_t)2 * cache[(i - 1) / 3];
                if (i >= 4) val += cache[(i - 4) / 3];
            } else { /* r == 2 */
                /* a(i) = a((i-2)/3) */
                if (i >= 2) val = cache[(i - 2) / 3];
            }
            cache[i] = val;
        }
    }
    return cache[n];
}
