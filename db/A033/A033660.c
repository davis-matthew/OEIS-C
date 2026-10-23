/*
 * A033660  Trajectory of 29 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + reverse(a(n-1)), a(0) = 29 Trajectory of 29
 *            under the map x -> x + reverse(x)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 99,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t len   = 0;   /* number of values computed */

    /* initialise cache with a(0) */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure: return zero */
        cache[0] = (seq_t)29;
        len = 1;
    }

    /* return memoised value if already computed */
    if (n < len) return cache[n];

    /* ensure enough capacity */
    if (cap <= n) {
        idx_t new_cap = cap > 0 ? cap * 2 : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return zero */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse(prev);  /* decimal reverse */
        cache[i]   = prev + rev;
    }
    len = n + 1;   /* update number of computed terms */

    return cache[n];
}
