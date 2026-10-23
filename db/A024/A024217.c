/*
 * A024217  a(n) = ( Product {k = 1..n} 3*k - 2 ) * ( Sum {k = 1..n}
 *          (-1)^(k+1)/(3*k - 2) ).
 * https://oeis.org/A024217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + (3*n-5)^2 * a(n-2) (n>=3), a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    /* Ensure the vector can hold indices 0..2 */
    if (cache.size() < 3) {
        cache.resize(3);
        cache[0] = 0;          /* unused */
        cache[1] = 1;
        cache[2] = 3;
    }
    /* Extend cache if needed */
    if (cache.size() <= n) {
        idx_t old_size = cache.size();
        cache.resize(n + 1);   /* new indices up to n are created */
        for (idx_t i = old_size; i <= n; ++i) {
            idx_t k = i - 1;  /* n in recurrence formula */
            seq_t a_prev = cache[i - 1];
            seq_t a_prev_prev = cache[i - 2];
            seq_t factor = (seq_t)(3 * k - 2);  /* (3*n-5) */
            seq_t factor_sq = factor * factor;
            cache[i] = a_prev * 3 + factor_sq * a_prev_prev;
        }
    }
    return cache[n];
#else
    /* C implementation using malloc */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated elements */

    /* Allocate at least 3 elements for indices 0..2 */
    if (cache_size < 3) {
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * 3);
        tmp[0] = 0;                /* unused */
        tmp[1] = 1;
        tmp[2] = 3;
        free(cache);
        cache = tmp;
        cache_size = 3;
    }

    /* Grow cache to hold n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * new_size);
        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t k = i - 1;
            seq_t a_prev = cache[i - 1];
            seq_t a_prev_prev = cache[i - 2];
            seq_t factor = (seq_t)(3 * k - 2);   /* (3*n-5) */
            seq_t factor_sq = factor * factor;
            cache[i] = a_prev * 3 + factor_sq * a_prev_prev;
        }
        cache_size = new_size;
    }

    return cache[n];
#endif
}
