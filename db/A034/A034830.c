/*
 * A034830  a(n) = n-th sept-factorial number divided by 3.
 * https://oeis.org/A034830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{j=1..n} (7*j-4)) / 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table of cumulative products of (7*j-4) for j=0..k; */
    static seq_t *prod_cache = NULL;   /* prod_cache[0] = 1, prod_cache[j] = ∏_{i=1..j}(7*i-4) */
    static idx_t  prod_cache_len = 0;   /* number of elements stored (max index + 1) */

    /* initialise the cache if it does not exist yet */
    if (!prod_cache) {
        prod_cache_len = 1;
        prod_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!prod_cache) return 0;   /* fallback on allocation failure */
        prod_cache[0] = (seq_t)1;
    }

    /* grow the cache to include n if necessary */
    if (n >= prod_cache_len) {
        idx_t new_len = n + 1;  /* we need indices 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(prod_cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;     /* fallback on allocation failure */
        prod_cache = tmp;
        /* compute missing products */
        for (idx_t j = prod_cache_len; j <= n; ++j) {
            seq_t term = (seq_t)(7 * j - 4);
            prod_cache[j] = prod_cache[j - 1] * term;
        }
        prod_cache_len = new_len;
    }

    /* a(n) = prod_cache[n] / 3  (integer division) */
    return prod_cache[n] / (seq_t)3;
}
