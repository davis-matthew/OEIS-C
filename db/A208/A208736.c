/*
 * A208736  Number of nonisomorphic graded posets with 0 and 1 and non-uniform
 *          Hasse graph of rank n, with exactly 2 elements of each rank level
 *          between 0 and 1.
 * https://oeis.org/A208736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) with a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=22
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized values */
    static idx_t cache_len = 0;      /* allocated size of cache */
    static idx_t computed_max = 5;   /* highest index already computed */

    /* initialise cache with the first six terms */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)22;
    }

    /* Return cached value if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Grow cache array if necessary */
    if ((idx_t)(n + 1) > cache_len) {
        size_t new_cap = cache_len;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_len = new_cap;
    }

    /* Compute new terms using the recurrence relation */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t t1 = ((seq_t)8) * cache[i - 1];
        seq_t t2 = ((seq_t)20) * cache[i - 3];
        seq_t t3 = ((seq_t)21) * cache[i - 2];
        seq_t t4 = ((seq_t)5)  * cache[i - 4];
        cache[i] = t1 + t2 - t3 - t4;
    }
    computed_max = n;
    return cache[n];
}
