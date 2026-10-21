/*
 * A077869  Expansion of (1-x)^(-1)/(1-x+x^3).
 * https://oeis.org/A077869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=2,a(2)=3,a(3)=3
   a(n)=2*a(n-1)-a(n-2)-a(n-3)+a(n-4) for n>=4
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */
        cache = tmp;

        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)2;
            else if (i == 2) cache[i] = (seq_t)3;
            else if (i == 3) cache[i] = (seq_t)3;
            else
                cache[i] =
                    (seq_t)((seq_t)2 * cache[i-1] - cache[i-2] - cache[i-3] + cache[i-4]);
        }
        cache_size = new_size;
    }
    return cache[n];
}
