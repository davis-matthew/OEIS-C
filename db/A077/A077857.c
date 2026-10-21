/*
 * A077857  Expansion of (1-x)^(-1)/(1-2*x+x^2+2*x^3).
 * https://oeis.org/A077857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=6, a(3)=8,
   a(n)=3*a(n-1)-3*a(n-2)-a(n-3)+2*a(n-4)   (n≥4) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0)      cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)3;
            else if (i == 2) cache[i] = (seq_t)6;
            else if (i == 3) cache[i] = (seq_t)8;
            else
                cache[i] = (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-2] - cache[i-3] + (seq_t)2 * cache[i-4];
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
