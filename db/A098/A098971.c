/*
 * A098971  a(0)=1; for n > 0, a(n)=a(floor(n/2))+2*a(floor(n/4)).
 * https://oeis.org/A098971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=a(floor(n/2))+2*a(floor(n/4))
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i) for 0 <= i < cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache has room up to n */
    if (cache == NULL) {
        /* initial allocation for index 0 */
        cache_size = 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure: return 0 */
        cache[0] = (seq_t)1; /* a(0) = 1 */
    }

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* floor division by 2 and 4 via shift */
            seq_t half = cache[i >> 1];
            seq_t quarter = cache[i >> 2];
            cache[i] = half + (seq_t)2 * quarter;
        }
        cache_size = new_size;
    }

    return cache[n];
}
