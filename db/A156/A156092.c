/*
 * A156092  Alternating sum of the squares of the first n Fibonacci numbers
 *          with index divisible by 4.
 * https://oeis.org/A156092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * F(4*n) * F(4*n+4) / 7. Recurrence used: a(0)=0,
 *            a(1)=-9, and for n≥2 a(n) = -47*a(n-1) - a(n-2) + (-1)^n*9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values of a(n) */
static idx_t cache_size = 0;     /* number of stored terms */

seq_t a(idx_t n)
{
    /* initialise cache for a(0) and a(1) */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-9;
    }

    /* grow and compute up to requested n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));

        /* compute terms from current cache_size up to n */
        for (idx_t k = cache_size; k <= n; ++k) {
            /* sign part: (+9) if k even, (-9) if k odd */
            seq_t sign = ((k & 1) == 0) ? (seq_t)9 : -(seq_t)9;
            /* recurrence: a(k) = -47*a(k-1) - a(k-2) + sign */
            seq_t term1 = cache[k-1];
            seq_t term2 = cache[k-2];
            seq_t val = -((seq_t)47 * term1) - term2 + sign;
            cache[k] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
