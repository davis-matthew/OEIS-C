/*
 * A116707  Number of permutations of length n which avoid the patterns 1342,
 *          4213.
 * https://oeis.org/A116707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 16*a(n-3) - 4*a(n-4) a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=22
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return directly for the first four terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)22;

    /* Static cache to support arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* allocated length */
    static idx_t max_computed = 0;   /* highest index already computed */

    /* Expand the cache if necessary. */
    if (n + 1 > cache_size) {
        size_t new_len = (size_t)(n + 5);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;    /* allocation failure */

        /* Copy previously computed values. */
        for (idx_t i = 1; i <= max_computed && i < new_len; ++i) {
            new_cache[i] = cache[i];
        }

        cache = new_cache;
        cache_size = (idx_t)new_len;
    }

    /* Initialize the base values if not yet set. */
    if (max_computed < 4) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)22;
        max_computed = 4;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = 7 * cache[i - 1];
        seq_t t2 = 16 * cache[i - 2];
        seq_t t3 = 16 * cache[i - 3];
        seq_t t4 = 4 * cache[i - 4];
        cache[i] = t1 - t2 + t3 - t4;
    }

    max_computed = n;
    return cache[n];
}
