/*
 * A091105  Expansion of (1-5x+40x^2)/((1-5x)(1+5x)).
 * https://oeis.org/A091105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-5, a(2)=65, a(n)=25*a(n-2) for n≥3
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed – unlikely */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)-5;
            else if (i == 2)
                cache[i] = (seq_t)65;
            else
                cache[i] = cache[i-2] * (seq_t)25;
        }
        cache_len = new_len;
    }
    return cache[n];
}
