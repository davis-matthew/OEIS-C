/*
 * A133216  Integers that are simultaneously triangular (A000217) and
 *          decagonal (A001107).
 * https://oeis.org/A133216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1)=0, a(2)=1, a(3)=10, a(4)=1540, a(5)=11935 * For n>5: a(n)
 *            = 1154*a(n-2) - a(n-4) + 396 * This uses a linear‑time
 *            recurrence and memoises results in a static cache
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms, 1‑based indexing.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t  cap      = 0;   /* allocated size (>=1) */
    static idx_t  computed = 0;   /* largest n already computed in cache */

    /* first two terms known explicitly (used when n <= 5) */
    static const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)10,
                                   (seq_t)1540, (seq_t)11935 };

    /* short‑circuit for the first five terms */
    if (n <= 5)
        return init[n - 1];

    /* grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;              /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failure */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (idx <= 5) {
            cache[idx] = init[idx - 1];
        } else {
            /* recurrence: a(i) = 1154*a(i-2) - a(i-4) + 396 */
            cache[idx] = (seq_t)1154 * cache[idx - 2] - cache[idx - 4] + (seq_t)396;
        }
    }

    computed = n;
    return cache[(size_t)n];
}
