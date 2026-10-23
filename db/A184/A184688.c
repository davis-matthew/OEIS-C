/*
 * A184688  1/3 the number of n X 3 0..2 arrays with no element equal both to
 *          the element above and to the element to its left.
 * https://oeis.org/A184688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9, a(2) = 192, a(n) = 22*a(n-1) - 16*a(n-2) for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache to allow arbitrary‑order calls. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* largest index currently computed */

    /* Initialize cache if first call. */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)(3) * sizeof(seq_t));  /* indices 0..2 */
        if (!cache)
            return (seq_t)0;   /* allocation failed – return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)9;   /* a(1) */
        cache[2] = (seq_t)192; /* a(2) */
        cache_max = 2;
    }

    /* Return cached value if already computed. */
    if (n <= cache_max)
        return cache[n];

    /* Grow the cache to hold the new index. */
    size_t new_cap = (size_t)(n + 2);   /* we need indices up to n */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;   /* allocation failed – return 0 */
    cache = tmp;

    /* Compute missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)22 * cache[i - 1];
        seq_t t2 = (seq_t)16 * cache[i - 2];
        cache[i] = t1 - t2;        /* safe: t1 ≥ t2 for these terms */
    }

    cache_max = n;
    return cache[n];
}
