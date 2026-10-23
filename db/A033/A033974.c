/*
 * A033974  Trajectory of 1 under map n->37n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = trajectory of 1 under the function f(x) = 37*x + 1 if x
 *            is odd, x/2 if x is even. a(0) = 1, and a(n+1) = f(a(n)) for n ≥
 *            0
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 46, u64 n <= 102, u128 n <= 155,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/seq_realloc.h"   /* seq_realloc */

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_len = 0;       /* number of computed terms */

    /* expand capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t need = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < need) new_cap = need;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[(size_t)0] = (seq_t)1;
        } else {
            seq_t prev = cache[(size_t)(i - 1)];
            if (prev % (seq_t)2 == 0) {               /* even */
                cache[(size_t)i] = prev / (seq_t)2;
            } else {                                  /* odd */
                cache[(size_t)i] = prev * (seq_t)37 + (seq_t)1;
            }
        }
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
