/*
 * A082561  a(1)=1, a(n)=3*a(n-1) if n is not a multiple of 3, or
 *          a(n)=a(n/3)+1 if n is a multiple of 3.
 * https://oeis.org/A082561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(n)=3*a(n-1) if n is not a multiple of 3, a(n)=a(n/3)+1
 *            if n is a multiple of 3. Offset: 1
 * Range:     u8 n <= 25, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation – index n-1 corresponds to a(n) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(1)=1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if (n > cache_len) {
        size_t needed = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;      /* allocation failure */
        cache = new_cache;

        /* compute a(i) for all i in [cache_len+1 .. n] */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i % 3ULL == 0ULL) {
                /* a(i) = a(i/3) + 1 */
                val = cache[(i / 3ULL) - 1] + (seq_t)1;
            } else {
                /* a(i) = 3 * a(i-1) */
                val = (seq_t)3 * cache[i - 2];
            }
            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
