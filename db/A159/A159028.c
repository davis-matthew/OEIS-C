/*
 * A159028  Numerator of Hermite(n, 7/8).
 * https://oeis.org/A159028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7*a(n-1) - 32*(n-1)*a(n-2), a(0)=1, a(1)=7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* store computed terms */
static idx_t cache_size = 0;     /* next free index in cache */

seq_t a(idx_t n)
{
    /* enlarge cache to hold n+1 terms */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed; return 0 as fallback */
        cache = tmp;
        /* fill new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)7;
            } else {
                seq_t term = 7 * cache[i - 1]
                           - (seq_t)(32 * (i - 1)) * cache[i - 2];
                cache[i] = term;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
