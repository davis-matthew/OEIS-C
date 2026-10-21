/*
 * A054884  Number of closed walks of length n along the edges of an
 *          icosahedron based at a vertex.
 * https://oeis.org/A054884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + 10*a(n-2) - 20*a(n-3) - 25*a(n-4)
 *  with a(0) = 1, a(1) = 0, a(2) = 5, a(3) = 10
 *  This recurrence gives the number of closed walks of length n
 *  on an icosahedron based at a vertex.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for n < 4 */
    static const seq_t base[4] = { (seq_t)1, (seq_t)0, (seq_t)5, (seq_t)10 };

    /* dynamic cache for memoisation (works for both C and C++ builds) */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n < 4)                        /* immediate return for the first four terms */
        return base[n];

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap  = 16;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure → 0 */
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache_size = 4;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache     = tmp;
            cache_cap = new_cap;
        } else {
            /* allocation failure → 0 (unlikely for requested n) */
            return (seq_t)0;
        }
    }

    /* compute missing terms up to n */
    if (cache_size <= (size_t)n) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];

            /* 4*a(n-1) + 10*a(n-2) - 20*a(n-3) - 25*a(n-4) */
            seq_t temp = a1 * 4 + a2 * 10;        /* positive contributions */
            seq_t sub  = a3 * 20 + a4 * 25;       /* negative contributions */
            cache[i] = temp - sub;                /* subtraction is fine for non‑negative result */
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
}
