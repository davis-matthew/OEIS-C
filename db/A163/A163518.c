/*
 * A163518  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163518
 *   a(0)=1, a(1)=23, a(2)=506, a(3)=11132, a(4)=244904, a(5)=5387635
 *   a(n) = 21*a(n-1)+21*a(n-2)+21*a(n-3)+21*a(n-4)-231*a(n-5)   for n ≥ 6
 *
 *  This implementation uses an O(n) iterative recurrence with memoisation.
 *  All arithmetic is performed in seq_t; no intermediate truncation is
 *  performed.  The routine is thread‑unsafe but complies with the build
 *  constraints. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)23,
        (seq_t)506,
        (seq_t)11132,
        (seq_t)244904,
        (seq_t)5387635
    };

    /* Coefficients as seq_t */
    static const seq_t C21  = (seq_t)21;
    static const seq_t C231 = (seq_t)231;

    /* Cache of computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* allocated size */
    static idx_t computed    = 5;   /* highest index computed so far */

    /* Return known initial term */
    if (n < 6)
        return init[n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();       /* allocation failure */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t s = cache[i-1] * C21;
            s += cache[i-2] * C21;
            s += cache[i-3] * C21;
            s += cache[i-4] * C21;
            s -= cache[i-5] * C231;
            cache[i] = s;
        }
        computed = n;
    }

    return cache[n];
}
