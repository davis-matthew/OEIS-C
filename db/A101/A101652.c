/*
 * A101652  a(n)=Product{k=0..n, 1+2^A010060(k)}/2.
 * https://oeis.org/A101652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   popparity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A101652
 *  a(n) = Product_{k=0..n} (1 + 2^A010060(k))/2
 *  Simplified recurrence: a(0)=1,
 *   a(n)=a(n-1) * (2 + (popcount(n) & 1))   for n>0
 */

#include <stdlib.h>
#include "../utils/popparity.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* initialise cache for n==0 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;

        /* compute new terms */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t factor = 2 + (seq_t)popparity(k);
            cache[k] = cache[k - 1] * factor;
        }
        cache_len = new_len;
    }

    return cache[n];
}
