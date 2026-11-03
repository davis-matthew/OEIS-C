/*
 * A096295  a(1) = 2, a(n) = a(n-1) + 3*(a(n-1)-floor(a(n-1)^(1/3))^3).
 * https://oeis.org/A096295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*(a(n-1)-floor(a(n-1)^(1/3))^3); a(1)=2
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
        cache = mem_new(seq_t, (size_t)n);
        if (!cache)
            exit(1);           /* out of memory */
    } else if ((size_t)n > cache_cap) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
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
