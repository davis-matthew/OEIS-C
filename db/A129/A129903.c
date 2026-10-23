/*
 * A129903  Expansion of 1/(1+x^2-x^3+x^4).
 * https://oeis.org/A129903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 49, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the expansion of 1/(1 + x^2 - x^3 + x^4):
   a(0) = 1
   a(1) = 0
   a(2) = -1
   a(3) = 1
   a(n) = -a(n-2) + a(n-3) - a(n-4)   (for n ≥ 4)
   (This satisfies (1 + x^2 - x^3 + x^4)·∑a(n)xⁿ = 1.) */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;                    /* number of computed elements */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;                   /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute any missing terms, starting from the first unknown index */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 4 && cache[i] != 0) continue;       /* already set base values */
        if (i == 0)          cache[0] =  1;
        else if (i == 1)     cache[1] =  0;
        else if (i == 2)     cache[2] = -1;
        else if (i == 3)     cache[3] =  1;
        else                 cache[i] = -cache[i-2] + cache[i-3] - cache[i-4];
    }

    return cache[n];
}
