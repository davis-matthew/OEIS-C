/*
 * A135401  a(n) = number of permutations (p(1),p(2),p(3),...,p(n)) of
 *          (1,2,3,...n) each of which is its own inverse and is such that
 *          p(k) = n + 1 - p(n+1-k) for all k in the range 1 <= k <= n.
 * https://oeis.org/A135401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000898(floor(n/2))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 29, u64 n <= 51, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000898 satisfies the recurrence: a_0 = 1, a_1 = 2,
 * a_{k+1} = 2*a_k + 2*k*a_{k-1}  (k >= 1). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* s = floor(n/2) */
    idx_t s = n >> 1;
    static seq_t *cache = NULL;      /* dynamic array of A000898 values   */
    static size_t cacheCap = 0;      /* allocated capacity of cache      */
    static size_t cacheSize = 0;     /* number of entries currently set   */

    /* Ensure the cache can hold indices up to s */
    if (cacheCap <= (size_t)s) {
        size_t newCap = (size_t)s + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cacheCap = newCap;
    }

    /* Compute missing values up to s */
    for (size_t i = cacheSize; i <= (size_t)s; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 2;
        } else {
            /* Recurrence: a_i = 2*a_{i-1} + 2*(i-1)*a_{i-2} */
            seq_t t1 = cache[i - 1] * 2;
            seq_t t2 = cache[i - 2] * ((seq_t)2 * (seq_t)(i - 1));
            cache[i] = t1 + t2;
        }
    }
    cacheSize = (size_t)s + 1;

    return cache[s];
}
