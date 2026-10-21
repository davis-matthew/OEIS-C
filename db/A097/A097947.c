/*
 * A097947  Expansion of g.f. (2+7*x+2*x^2)/((x^2-1)*(1+4*x+x^2)).
 * https://oeis.org/A097947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -2, a(1) = 1, a(2) = -6, a(3) = 16
   a(n) = a(n-4) + 4*a(n-3) - 4*a(n-1)  for n >= 4   */
/* Implements A097947: Expansion of (2+7x+2x²)/((x²-1)(1+4x+x²)) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len)
        return cache[n];

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failure – not expected */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)(-2);
        else if (i == 1)
            cache[1] = (seq_t)(1);
        else if (i == 2)
            cache[2] = (seq_t)(-6);
        else if (i == 3)
            cache[3] = (seq_t)(16);
        else {
            /* n >= 4: use the recurrence */
            seq_t val = cache[i - 4];
            val += (seq_t)4 * cache[i - 3];
            val -= (seq_t)4 * cache[i - 1];
            cache[i] = val;
        }
    }
    cache_len = new_len;
    return cache[n];
}
