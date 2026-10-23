/*
 * A129379  a(n) = sum of sums of all sets of three distinct preceding terms
 *          otherwise a(n) = n for n <= 3.
 * https://oeis.org/A129379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)(n-2)(n-3)*Sum_{j=1}^{n-1} a(j) (n>3),
 *            a(1)=1,a(2)=2,a(3)=3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static idx_t  cache_sz = 0;          /* highest n for which cache[n] is valid */
    static size_t cache_cap = 0;          /* allocated capacity (>= cache_sz+1) */
    static seq_t prefix_sum = 0;          /* Sum_{j=1}^{cache_sz} a(j) */

    if (n == 0) return 0;                 /* safety; offset begins at 1 */

    /* Grow the cache array if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        /* cast is required for C++ mode where seq_t may be a class */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if they are missing */
    if (n > cache_sz) {
        for (idx_t k = cache_sz + 1; k <= n; ++k) {
            seq_t val;
            if (k <= 3) {
                val = (seq_t)k;
            } else {
                seq_t num    = (seq_t)(k - 2) * (seq_t)(k - 3);
                seq_t factor = num / 2;            /* (n-2)(n-3)/2   */
                val = factor * prefix_sum;
            }
            cache[k] = val;
            prefix_sum += val;
        }
        cache_sz = n;
    }

    return cache[n];
}
