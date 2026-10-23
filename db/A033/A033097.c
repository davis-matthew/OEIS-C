/*
 * A033097  Number of 2's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=2}^{n+1} (number of digits equal to 2 in the base‑b
 *            representation of n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t, NULL */

seq_t a(idx_t n)
{
    /* Offset protection: a(1)=0 for n=1, but the cache is 1‑based. */
    if (n == 0)
        return 0;

    /* Static cache array: cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;     /* allocated storage, size = cap */
    static idx_t   cap   = 0;       /* allocated capacity (≥1 if cache!=NULL) */
    static idx_t   size  = 0;       /* highest n computed so far (0 if none) */

    /* Return cached value if already computed */
    if (n <= size)
        return cache[n];

    /* Grow the cache if necessary */
    if (cap < n + 1) {
        /* Ensure capacity is at least n+1 */
        cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort with zero (undefined behaviour in proper build) */
            return 0;
        }
        cache = tmp;
    }

    /* Compute missing values from size+1 up to n */
    for (idx_t k = size + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t b = 2; b <= k + 1; ++b) {
            idx_t t = k;
            /* Count digits equal to 2 in base b */
            while (t) {
                if ((t % b) == 2u)
                    ++sum;
                t /= b;
            }
        }
        cache[k] = sum;
    }
    size = n;   /* update highest computed index */

    return cache[n];
}
