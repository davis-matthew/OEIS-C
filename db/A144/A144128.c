/*
 * A144128  Chebyshev U(n,x) polynomial evaluated at x=18.
 * https://oeis.org/A144128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*a(n‑1) - a(n‑2), with a(1)=1, a(2)=36
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External linkage is required */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 in that case */
    if (n < 1U)
        return (seq_t)0;

    /* Cache of previously computed terms.
       cache[0] = a(1), cache[1] = a(2), ...  */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* current cache length */

    /* Initialise the cache with the first two terms */
    if (size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (cache == NULL)          /* allocation failure – fall back */
            return (seq_t)0;
        cache[0] = (seq_t)1;        /* a(1) */
        cache[1] = (seq_t)36;       /* a(2) */
        size = 2;
    }

    /* Grow the cache if the requested index is beyond what we have */
    if (n > size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (tmp == NULL)          /* allocation failure – fall back */
            return (seq_t)0;
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t prev1 = cache[i - 2];   /* a(i‑1) */
            seq_t prev2 = cache[i - 3];   /* a(i‑2) */
            cache[i - 1] = ((seq_t)36 * prev1) - prev2;
        }
        size = n;
    }

    return cache[n - 1];
}
