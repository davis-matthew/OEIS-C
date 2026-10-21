/*
 * A099743  Number of permutations with exactly 1 valley which avoid the
 *          pattern 1324.
 * https://oeis.org/A099743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 30*a(n-2) + 44*a(n-3) - 24*a(n-4) (for n > 4,
 *            a(1)=0, a(2)=0, a(3)=2, a(4)=15)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms already computed */

    /* initialize first four terms */
    if (cache_len == 0)
    {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)2;   /* a(3) */
        cache[3] = (seq_t)15;  /* a(4) */
    }

    /* compute missing terms up to n */
    if ((size_t)n > cache_len)
    {
        /* enlarge the cache */
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        for (size_t k = cache_len; k < (size_t)n; ++k)
        {
            seq_t t0 = cache[k - 1]; /* a(k)   */
            seq_t t1 = cache[k - 2]; /* a(k-1) */
            seq_t t2 = cache[k - 3]; /* a(k-2) */
            seq_t t3 = cache[k - 4]; /* a(k-3) */

            seq_t new_val = (seq_t)9 * t0 - (seq_t)30 * t1
                          + (seq_t)44 * t2 - (seq_t)24 * t3;
            cache[k] = new_val;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
