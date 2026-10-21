/*
 * A096295  a(1) = 2, a(n) = a(n-1) + 3*(a(n-1)-floor(a(n-1)^(1/3))^3).
 * https://oeis.org/A096295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*(a(n-1)-floor(a(n-1)^(1/3))^3); a(1)=2
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_cube_root.h"

static seq_t *cache = NULL;      /* array of computed a(i) values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t  last_index = 0;    /* number of terms already computed */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n<1, just in case */
        return 0;

    /* If the requested value is already cached, use it */
    if (n <= last_index)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!cache)
            exit(1);           /* out of memory */
    } else if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
    }
    cache_cap = (size_t)n;

    /* Compute all missing terms up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
        } else {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t m    = floor_cube_root(prev); /* floor(cuberoot(a(i-1))) */
            seq_t cube = m * m * m;
            seq_t diff = prev - cube;
            seq_t next = prev + 3 * diff;       /* a(i) */
            cache[i - 1] = next;
        }
    }

    last_index = n;
    return cache[n - 1];
}
