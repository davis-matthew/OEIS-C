/*
 * A134683  Expansion of 1+x*(2+3*x)/(1-4*x^2).
 * https://oeis.org/A134683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3; for n>2, a(n)=4*a(n-2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Handle base case directly. */
    if (n == 0)
        return (seq_t)1;

    /* Static cache: cache[0 .. cached_size] holds already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* highest index computed so far */

    /* Initialise cache with the first three terms. */
    if (!cache) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                 /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cached_size = 2;
    }

    /* Return the value if it has already been computed. */
    if (n <= cached_size)
        return cache[n];

    /* Grow the cache to accommodate index n. */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                 /* allocation failed */
    cache = tmp;

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cached_size + 1; i <= n; ++i) {
        /* The recurrence a(i) = 4*a(i-2) holds for all i >= 3. */
        cache[i] = cache[i - 2] * (seq_t)4;
    }
    cached_size = n;

    return cache[n];
}
