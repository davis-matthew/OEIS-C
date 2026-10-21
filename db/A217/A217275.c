/*
 * A217275  Expansion of 2/(1-x+sqrt(1-2*x-27*x^2)).
 * https://oeis.org/A217275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recurrence: (n+2)*a(n) = (2*n+1)*a(n-1) + 27*(n-1)*a(n-2),
   with a(0) = 1, a(1) = 1 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Allocate initial cache */
    if (cache == NULL) {
        size_t init = n >= 1 ? (size_t)n + 1 : 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * init);
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = init;
        max_computed = 1;
    }

    /* Resize if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Return if already computed */
    if (n <= max_computed) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
#ifdef __cplusplus
        seq_t term1 = (seq_t)(2 * i + 1) * cache[i - 1];
        seq_t term2 = (seq_t)(27 * (i - 1)) * cache[i - 2];
        cache[i] = (term1 + term2) / (seq_t)(i + 2);
#else
        __int128 t1 = (__int128)cache[i - 1] * (2 * i + 1);
        __int128 t2 = (__int128)cache[i - 2] * (27 * (i - 1));
        cache[i] = (seq_t)((t1 + t2) / (i + 2));
#endif
    }

    max_computed = n;
    return cache[n];
}
