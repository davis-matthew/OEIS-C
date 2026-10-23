/*
 * A032798  Numbers such that n(n+1)(n+2)...(n+12) /
 *          (n+(n+1)+(n+2)+...+(n+12)) is a multiple of n.
 * https://oeis.org/A032798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12; initial values
 *            a(1)=1,a(2)=2,a(3)=3,a(4)=4,a(5)=5,a(6)=6,a(7)=8,a(8)=9,a(9)=10,a(10)=11,a(11)=12,a(12)=14
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached terms */
static seq_t *cache      = NULL;   /* array of computed terms */
static idx_t cache_size  = 0;      /* number of terms currently stored (>= cache_cap) */
static idx_t cache_cap   = 0;      /* allocated capacity */

/* Base values for n = 1 .. 12 */
static const seq_t base[12] = {
    1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14
};

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* not part of the sequence */
    /* Initialize cache if necessary */
    if (cache == NULL) {
        cache_cap = 16;                   /* initial capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache_size = 0;
    }
    /* Grow cache if needed */
    while (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }
    /* Compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[(size_t)i - 1] = base[(size_t)i - 1];
        } else {
            /* use recurrence: a(n) = a(n-1) + a(n-11) - a(n-12) */
            cache[(size_t)i - 1] = cache[(size_t)i - 2]
                                  + cache[(size_t)i - 12]
                                  - cache[(size_t)i - 13];
        }
    }
    cache_size = n;
    return cache[(size_t)n - 1];
}
