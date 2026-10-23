/*
 * A167449  a(0)=1; a(1)=1; for a>1, a(n)=a(n-1)+((n-1)^3)*a(n-2).
 * https://oeis.org/A167449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=1; for n>1, a(n)=a(n-1)+((n-1)^3)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;   /* allocated length of cache   */
    static idx_t   last_idx  = 0;   /* highest index that has been computed */

    /* initialise cache for the first call */
    if (cache == NULL) {
        cached_len = 2;                           /* at least a[0] and a[1]   */
        cache = (seq_t *)malloc(sizeof(seq_t) * cached_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_idx = 1;
    }

    /* grow the cache if n exceeds the current allocation */
    if ((size_t)n >= cached_len) {
        size_t new_len = cached_len;
        while ((size_t)n >= new_len) new_len <<= 1;   /* double until enough */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        memcpy(new_cache, cache, cached_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cached_len = new_len;
    }

    /* compute values up to n if needed */
    if (n > last_idx) {
        for (idx_t i = last_idx + 1; i <= n; ++i) {
            seq_t base = (seq_t)(i - 1);
            seq_t cube = base * base * base;
            cache[i] = cache[i - 1] + cube * cache[i - 2];
        }
        last_idx = n;
    }

    return cache[n];
}
