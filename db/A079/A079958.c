/*
 * A079958  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={3,4}.
 * https://oeis.org/A079958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-6) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=13
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* Base cases for 0..5 */
    const seq_t base[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                            (seq_t)4, (seq_t)7, (seq_t)13 };

    /* Return directly for n < 6 */
    if (n < 6) return base[n];

    /* Grow cache to accommodate index n */
    if (cache_size <= n) {
        /* Determine new size (at least n+1, double each step) */
        idx_t new_size = (cache_size == 0) ? 16 : cache_size * 2;
        while (new_size <= n) new_size <<= 1;

        seq_t *new_cache;
#ifdef __cplusplus
        /* C++: use new[] and delete[] to ensure proper construction */
        new_cache = new seq_t[new_size];
        /* Copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* Replace old cache */
        delete[] cache;
#else
        /* C: use realloc (no constructors needed) */
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
#endif
        /* Compute values for indices that are newly added */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 6)
                new_cache[i] = base[i];
            else
                new_cache[i] = new_cache[i-1] + new_cache[i-2] +
                                new_cache[i-3] + new_cache[i-6];
        }
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
