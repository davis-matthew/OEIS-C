/*
 * A195254  O.g.f.: Sum_{n>=0} 2*(n+2)^(n-1)*x^n/(1+n*x)^n.
 * https://oeis.org/A195254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A195254
   Formula: a(0) = 1
            a(1) = 2
            a(n) = (n+1)*a(n-1) - 2*(n-2)*a(n-2)   for n > 1
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure space for terms 0..n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure → abort */
        cache = tmp;

        /* compute missing values */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else {
                seq_t term1 = (seq_t)(i + 1) * cache[i - 1];
                seq_t term2 = (seq_t)2 * (seq_t)(i - 2) * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
