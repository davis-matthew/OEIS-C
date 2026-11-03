/*
 * A097829  Partial sums of Chebyshev sequence S(n,15) = U(n,15/2) =
 *          A078364(n).
 * https://oeis.org/A097829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - a(n-2) + 1 (n ≥ 2) a(0) = 1, a(1) = 16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size of cache   */
    static idx_t last   = (idx_t)-1; /* index of last computed value */

    /* allocate or enlarge cache if needed */
    if (cache == NULL) {
        /* initial capacity: at least 2 to hold a(0) and a(1) */
        cap = 2;
        if (n + 1 > cap) cap = (size_t)n + 1;
        cache = mem_new(seq_t, cap);
        if (cache == NULL) abort();      /* allocation failure */
        cache[0] = (seq_t)1;             /* a(0) */
        cache[1] = (seq_t)16;            /* a(1) */
        last = (n >= 1 ? 1 : 0);
    } else if ((size_t)n + 1 > cap) {
        /* grow cache to at least n+1 */
        size_t new_cap = cap * 2;
        if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();       /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;        /* a(0) */
        } else if (i == 1) {
            cache[i] = (seq_t)16;       /* a(1) */
        } else {
            /* a(i) = 15*a(i-1) - a(i-2) + 1 */
            cache[i] = (seq_t)15 * cache[i - 1] - cache[i - 2] + 1;
        }
    }
    last = n;
    return cache[n];
}
