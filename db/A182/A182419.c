/*
 * A182419  a(0)=0, a(n+1) = (a(n) XOR floor(a(n)/8)) + 1, where XOR is the
 *          bitwise exclusive-or operator.
 * https://oeis.org/A182419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(n+1) = ( a(n) XOR floor(a(n)/8) ) + 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms. */
    static seq_t *cache = NULL;        /* array of terms */
    static idx_t cache_len = 0;        /* allocated length of cache   */
    static idx_t max_computed = 0;     /* highest index stored in cache */

    /* First call: initialise cache with a(0)=0. */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();           /* allocation failed */
        cache[0] = (seq_t)0;
        max_computed = 0;
    }

    /* Ensure allocated space is enough to hold a[n]. */
    if (n + 1 > cache_len) {
        idx_t new_len = cache_len ? cache_len << 1 : 1;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();             /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (prev ^ (prev >> 3)) + (seq_t)1;
    }
    max_computed = n;

    return cache[n];
}
