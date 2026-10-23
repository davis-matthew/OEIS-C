/*
 * A140948  a(0) = 3; for n >= 1, if a(n-1) = 2*k, then a(n) = k, otherwise 1
 *          + (A065091(n)*a(n-1)), where A065091(n) gives the n-th odd prime.
 * https://oeis.org/A140948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3; a(n)=If[Mod[a(n-1),2]==0,a(n-1)/2,Prime(n+1)*a(n-1)+1]
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 52, u128 n <= 85,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 ≤ i < cache_len */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure enough space */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        cache_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)3;
        } else {
            seq_t prev = cache[i - 1];
            if ((prev & (seq_t)1) == 0) {          /* even */
                cache[i] = prev >> 1;
            } else {                               /* odd */
                uint32_t odd_prime = get_prime((idx_t)(i + 1));
                cache[i] = prev * (seq_t)odd_prime + (seq_t)1;
            }
        }
    }
    cache_len = needed;

    return cache[(size_t)n];
}
