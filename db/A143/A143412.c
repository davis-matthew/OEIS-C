/*
 * A143412  Main diagonal of A143410.
 * https://oeis.org/A143412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(2*n-1)*a(n-1) + a(n-2); a(0)=1, a(1)=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t upto = 0;       /* next index to compute */

    /* allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = seq_alloc_array(cache_cap);
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < upto; ++i) new_cache[i] = cache[i];
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    while (upto <= (size_t)n) {
        if (upto == 0) {
            cache[0] = (seq_t)1;
            upto = 1;
        } else if (upto == 1) {
            cache[1] = (seq_t)3;
            upto = 2;
        } else {
            idx_t k = upto;
            seq_t mul = ((seq_t)4) * ((((seq_t)2) * (seq_t)k) - ((seq_t)1));
            cache[k] = mul * cache[k-1] + cache[k-2];
            upto = k + 1;
        }
    }

    return cache[n];
}
