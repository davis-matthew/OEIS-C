/*
 * A085372  If n = 0 or 1 then a(n) = 1; if n = 2, 3 or 4 then a(n) = 0;
 *          otherwise a(n) =
 *          (n+1)*a(n-1)-(n-2)*a(n-2)-(n-5)*a(n-3)+(n-3)*a(n-4).
 * https://oeis.org/A085372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 0, a(3) = 0, a(4) = 0 For n ≥ 5: a(n)
 *            = (n+1)*a(n-1) - (n-2)*a(n-2) - (n-5)*a(n-3) + (n-3)*a(n-4)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t last_computed = 4;   /* highest index already computed */

    /* initial allocation and base values */
    if (cache == NULL) {
        /* allocate space for indices 0 … 4 */
        cache = (seq_t *)malloc(sizeof(seq_t) * (last_computed + 1));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
    }

    /* if the requested n is already in the cache, return it directly */
    if (n <= last_computed)
        return cache[n];

    /* extend the cache up to n */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);

    /* compute new terms sequentially up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence uses only terms up to i‑1, which are now computed */
        seq_t term = ((seq_t)(i + 1)) * cache[i - 1]
                   - ((seq_t)(i - 2)) * cache[i - 2]
                   - ((seq_t)(i - 5)) * cache[i - 3]
                   + ((seq_t)(i - 3)) * cache[i - 4];
        cache[i] = term;
    }

    /* update the highest computed index */
    last_computed = n;
    return cache[n];
}
