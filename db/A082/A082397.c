/*
 * A082397  Number of directed aggregates of height <= 2 with n cells.
 * https://oeis.org/A082397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = ((3n + 2) * a(n-1) + n * a(n-2) - 3n * a(n-3)) / (n + 2)
   Initial values: a(1) = 1, a(2) = 2, a(3) = 5 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms; 1-based indexing (index 0 unused) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* highest n stored so far */

    if (n == 0) return (seq_t)0;         /* undefined, harmless */

    /* initialisation of the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        cache[1] = (seq_t)1;  /* a(1) */
        cache[2] = (seq_t)2;  /* a(2) */
        cache[3] = (seq_t)5;  /* a(3) */
    }

    /* if already computed, just return */
    if (n <= cache_len) return cache[n];

    /* extend cache to accommodate up to n */
    size_t new_size = (size_t)(n + 1);   /* +1 because index 0 unused */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* compute new terms using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t term1 = (seq_t)(3 * k + 2) * cache[k - 1];
        seq_t term2 = (seq_t)(k) * cache[k - 2];
        seq_t term3 = (seq_t)(3 * k) * cache[k - 3];
        seq_t numerator = term1 + term2;
        numerator -= term3;
        seq_t denom = (seq_t)(k + 2);
        cache[k] = numerator / denom;
    }

    cache_len = n;
    return cache[n];
}
