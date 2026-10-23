/*
 * A177170  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=0 and l=-2.
 * https://oeis.org/A177170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=8. For n≥1: a(n+1)=∑_{p=0}^{n} a(p)*a(n−p)−2 (i.e.
 *            convolution minus 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdlib.h>
# include "../utils/alloc_seq_array.h"
# include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* initialise with the two known terms */
    if (cached_len == 0) {
        cache = alloc_seq_array(2);     /* allocate array of size 2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cached_len = 2;
    }

    /* return already computed value */
    if (n < cached_len)
        return cache[n];

    /* grow the cache and compute all missing terms */
    idx_t new_len = n + 1;
    seq_t *new_cache = alloc_seq_array(new_len);

    /* copy existing terms */
    for (idx_t i = 0; i < cached_len; ++i)
        new_cache[i] = cache[i];

    /* compute remaining terms using the recurrence */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t p = 0; p <= i - 1; ++p)
            sum += new_cache[p] * new_cache[i - 1 - p];
        new_cache[i] = sum - (seq_t)2;   /* subtract 2 (l = -2) */
    }

    /* replace old cache */
    free_seq_array(cache);
    cache = new_cache;
    cached_len = new_len;

    return cache[n];
}
