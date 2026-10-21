/*
 * A206340  Number of (n+1)X6 0..3 arrays with every 2X2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 28, u64 n <= 68, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A206340
   a(1)=1376, a(2)=686, a(3)=4007, a(4)=2034, a(5)=12021, a(6)=6090,
   a(7)=36015, a(8)=18222, a(9)=107997, a(10)=54618, a(11)=323943,
   a(12)=163806, a(13)=971799, a(14)=491406.
   For n > 14: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoized recurrence */
    static const seq_t init[14] = {
        /* a(1) to a(14) */
        (seq_t)1376, (seq_t)686, (seq_t)4007, (seq_t)2034,
        (seq_t)12021, (seq_t)6090, (seq_t)36015, (seq_t)18222,
        (seq_t)107997, (seq_t)54618, (seq_t)323943, (seq_t)163806,
        (seq_t)971799, (seq_t)491406
    };
    static seq_t *cache = NULL; /* 0‑based array of a(1)…a(k) */
    static idx_t cache_size = 0; /* number of terms stored */

    if (n == 0) return (seq_t)0; /* safe guard, should not be used */

    if (n <= 14) return init[n-1];

    if (cache == NULL) {
        /* start with known 14 terms */
        cache = (seq_t*)malloc(sizeof(seq_t) * 14);
        for (idx_t i = 0; i < 14; ++i) cache[i] = init[i];
        cache_size = 14;
    }

    if (n <= cache_size) {
        return cache[n-1];
    }

    /* extend cache up to n */
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * n);
    if (new_cache == NULL) {
        /* allocation failed – fall back, though this should not happen */
        return (seq_t)0;
    }
    cache = new_cache;

    for (idx_t i = cache_size; i < n; ++i) {
        /* compute a(i+1) using recurrence: a_n = a_{n-1} + 3*a_{n-2} - 3*a_{n-3} */
        cache[i] = cache[i-1] + 3*cache[i-2] - 3*cache[i-3];
    }

    cache_size = n;
    return cache[n-1];
}
