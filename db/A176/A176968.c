/*
 * A176968  Expansion of x*( 1+2*x-x^2-6*x^3 ) / ( 1-9*x^2+12*x^4 ).
 * https://oeis.org/A176968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula:
   a(1) = 1, a(2) = 2
   a(n) = a(n-1) + 6*a(n-2)  if n is odd
   a(n) = a(n-1) + 2*a(n-2)  if n is even
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return (seq_t)0;    /* undefined for n<1, return 0 */

    /* allocate or grow cache if needed */
    if (n > cache_size) {
        idx_t new_size = n + 1;   /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp)                /* allocation failed */
            return (seq_t)0;
        cache = tmp;

        /* ensure base values a(1) and a(2) are set if the array was
           just expanded from a smaller size or was previously empty */
        if (cache_size < 1) {
            cache[1] = (seq_t)1;
            cache_size = 1;
        }
        if (cache_size < 2 && n >= 2) {
            cache[2] = (seq_t)2;
            cache_size = 2;
        }

        /* compute any new terms up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if ((i & 1) == 1)   /* odd */
                cache[i] = cache[i-1] + (seq_t)6 * cache[i-2];
            else                /* even */
                cache[i] = cache[i-1] + (seq_t)2 * cache[i-2];
        }

        cache_size = n;
    }

    return cache[n];
}
