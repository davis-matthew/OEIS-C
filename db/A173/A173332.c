/*
 * A173332  a(n) = a(n-2) + a(n-3) - floor(a(n-4)/2) - floor(a(n-6)/2).
 * https://oeis.org/A173332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 52, i16 n <= 132, i32 n <= 254, i64 n <= 493,
 *            i128 n <= 975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A173332
 *
 * Formula:
 *   a(n) = a(n-2) + a(n-3) - ⌊a(n-4)/2⌋ - ⌊a(n-6)/2⌋
 *   with initial values
 *     a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=3, a(6)=4
 *
 * The function caches results so that calls in any order run in O(n)
 * time for the largest n requested so far.
 */

#include <stdlib.h>

/* Integer division by 2 with floor semantics (handles negative numbers). */
static inline seq_t floor_div2(seq_t x)
{
    return (x >= 0) ? (x / 2) : ((x - 1) / 2);
}

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* array of values */
    static size_t    cap   = 0;     /* allocated size of cache */
    static idx_t     size  = 0;     /* number of values currently stored */

    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – hard‑coded fallback (unlikely in tests) */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        switch (i) {
        case 0: val = 1; break;
        case 1: val = 1; break;
        case 2: val = 1; break;
        case 3: val = 2; break;
        case 4: val = 2; break;
        case 5: val = 3; break;
        case 6: val = 4; break;
        default:
            val = cache[i - 2] + cache[i - 3]
                  - floor_div2(cache[i - 4])
                  - floor_div2(cache[i - 6]);
        }
        cache[i] = val;
    }
    size = n + 1;

    return cache[n];
}
