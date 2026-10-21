/*
 * A221954  a(n) = 3^(n-1) * n! * Catalan(n-1).
 * https://oeis.org/A221954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(2*n-3)*a(n-1), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1, return 0 as fallback */

    if (cache_max == 0) {
        /* initialize cache with a(1) */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache)
            return 0;
        cache[0] = (seq_t)1;
        cache_max = 1;
    }

    if (n > cache_max) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)
            return 0;
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* factor = 6 * (2*i - 3) */
            seq_t factor = (seq_t)12 * (seq_t)i - (seq_t)18;
            cache[i - 1] = factor * cache[i - 2];
        }
        cache_max = n;
    }

    return cache[n - 1];
}
