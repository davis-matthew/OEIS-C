/*
 * A239488  Expansion of 1/x-4/(-sqrt(x^2-10*x+1)-x+1)-3.
 * https://oeis.org/A239488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*(2*n-1)*a(n-1) - (n-2)*a(n-2))/(n+1) with a(1) = 6,
 *            a(2) = 30. Uses memoisation
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 2) return (n == 1 ? (seq_t)6 : (seq_t)30);

    /* static cache to reuse previously computed terms */
    static seq_t *cache = NULL;      /* cache[0] unused, index 1.. */
    static idx_t cap = 0;            /* current allocated size (≥ n) */

    /* allocate or grow cache */
    if (n >= cap) {
        idx_t new_cap = n + 1;       /* one more than needed for index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                  /* allocation failed – abort */
            abort();
        }
        /* initialise new elements to 0 (optional but safe) */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first two terms once */
    if (cache[1] == 0) cache[1] = (seq_t)6;
    if (cache[2] == 0) cache[2] = (seq_t)30;

    /* compute missing terms up to n */
    for (idx_t k = 3; k <= n; ++k) {
        if (cache[k] == 0) {
            seq_t term1 = (seq_t)5 * (seq_t)(2 * k - 1) * cache[k - 1];
            seq_t term2 = (seq_t)(k - 2) * cache[k - 2];
            cache[k] = (term1 - term2) / (seq_t)(k + 1);
        }
    }

    return cache[n];
}
