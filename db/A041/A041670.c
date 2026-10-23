/*
 * A041670  Numerators of continued fraction convergents to sqrt(354).
 * https://oeis.org/A041670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 516130*a(n-10) - a(n-20) (n > 19) a(0) = 18, a(1) = 19,
 *            a(2) = 94, a(3) = 207, a(4) = 508, a(5) = 9351, a(6) = 19210,
 *            a(7) = 47771, a(8) = 210294, a(9) = 258065, a(10) = 9500634,
 *            a(11) = 9758699, a(12) = 48535430, a(13) = 106829559, a(14) =
 *            262194548, a(15) = 4826331423, a(16) = 9914857394...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Initial 20 values */
static const seq_t init[20] = {
    18, 19, 94, 207, 508,
    9351, 19210, 47771, 210294, 258065,
    9500634, 9758699, 48535430, 106829559, 262194548,
    4826331423, 9914857394, 24656046211, 108539042238, 133195088449
};

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* For n < 20 return the stored constant */
    if (n < 20u) {
        return init[(size_t)n];
    }

    /* Grow the cache to at least n+1 entries */
    if (cap <= n) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* OOM guard – unlikely */

        /* Initialise new entries */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i < 20u) {
                new_cache[i] = init[i];
            } else {
                new_cache[i] = new_cache[i - 10] * 516130u - new_cache[i - 20];
            }
        }
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n];
}
