/*
 * A045674  Number of 2n-bead balanced binary necklaces which are equivalent
 *          to their reverse, complement and reversed complement.
 * https://oeis.org/A045674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(2n)=a(n)+2^(n-1) for n>0; a(2n+1)=2^n
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 200, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first term when first called */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;
        cache_size = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }
            if (i & 1U) {                    /* odd: i = 2k+1 */
                idx_t k = (i - 1) >> 1;
                cache[i] = big_pow((seq_t)2, (seq_t)k);
            } else {                         /* even: i = 2k */
                idx_t k = i >> 1;
                seq_t pow_term = big_pow((seq_t)2, (seq_t)(k - 1));
                cache[i] = cache[k] + pow_term;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
