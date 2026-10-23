/*
 * A165374  Number of slanted n X 5 (i=1..n) X (j=i..5+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, and 4 in the lower right
 *          corner.
 * https://oeis.org/A165374
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2) + 38*a(n-3) - 28*a(n-4) + 8*a(n-5)
 *            (n ≥ 11) Initial values (OEIS offset 2): a(2) = 105, a(3) =
 *            1622, a(4) = 11960, a(5) = 63740, a(6) = 272548, a(7) = 993780,
 *            a(8) = 3239860, a(9) = 9743508, a(10) = 27593908, a(11) =
 *            74632788
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 43, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the official offset: return 0 (undefined) */
    if (n < 2) return 0;

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;          /* number of elements stored (>= n+1 when initialized) */

    /* Initial values for n = 2..11 */
    static const seq_t init_vals[12] = {
        0,          /* 0 */
        0,          /* 1 */
        105,        /* 2 */
        1622,       /* 3 */
        11960,      /* 4 */
        63740,      /* 5 */
        272548,     /* 6 */
        993780,     /* 7 */
        3239860,    /* 8 */
        9743508,    /* 9 */
        27593908,   /*10*/
        74632788    /*11*/
    };

    /* Allocate or grow cache to contain indices up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure: return 0 */
        cached_len = n + 1;
    } else if (cached_len <= (size_t)n) {
        /* Expand cache */
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* realloc failure: return 0 */
        cache = tmp;
        cached_len = new_len;
    }

    /* Compute missing values up to n */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (i <= 11) {
            cache[i] = init_vals[i];
        } else {
            seq_t t1 = 8 * cache[i-1];
            seq_t t2 = 25 * cache[i-2];
            seq_t t3 = 38 * cache[i-3];
            seq_t t4 = 28 * cache[i-4];
            seq_t t5 = 8  * cache[i-5];
            cache[i] = t1 - t2 + t3 - t4 + t5;
        }
    }

    return cache[n];
}
