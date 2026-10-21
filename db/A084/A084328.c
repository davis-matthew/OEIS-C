/*
 * A084328  a(0)=0, a(1)=1; a(n) = 13*a(n-1) - 11*a(n-2).
 * https://oeis.org/A084328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1; a(n) = 13*a(n-1) - 11*a(n-2) for n >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of already computed terms   */
    static idx_t  cache_size = 0;        /* allocated size of cache          */
    static idx_t  last_computed = (idx_t)-1; /* highest index that has been computed */

    /* grow cache if necessary */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;        /* memory failure – unlikely in tests */
        cache = new_cache;
        cache_size = n + 1;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t t1 = (seq_t)13 * cache[i - 1];
            seq_t t2 = (seq_t)11 * cache[i - 2];
            cache[i] = t1 - t2;
        }
    }
    last_computed = n;
    return cache[n];
}
