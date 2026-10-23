/*
 * A131765  Series reversion of x*(1-5x)/(1-x) .
 * https://oeis.org/A131765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n) = 9*(2*n-1)*a(n-1) - (n-2)*a(n-2) Initial values:
 *            a(0)=1, a(1)=4 Recurrence is valid for n ≥ 2. The function
 *            memoises previously computed terms and works for any sequence
 *            type seq_t supplied by the build
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   term_from_recurrence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/term_from_recurrence.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */

    /* initialise cache with the first two terms */
    if (cap == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* OOM */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* if the requested index is already cached, return it */
    if (n < (idx_t)cap) {
        return cache[(size_t)n];
    }

    /* ensure the cache is large enough */
    size_t old_cap = cap;
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            return (seq_t)0;      /* OOM */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = old_cap; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        cache[i] = term_from_recurrence(i, a_nm1, a_nm2);
    }

    return cache[(size_t)n];
}
