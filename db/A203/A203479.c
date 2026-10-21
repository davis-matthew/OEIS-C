/*
 * A203479  a(n) = Product_{1 <= i < j <= n} (2^i + 2^j - 2).
 * https://oeis.org/A203479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (2^i + 2^j – 2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* cache for previously computed sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* cache for powers of two up to the current n */
    static seq_t *pow2_arr = NULL;
    static idx_t pow2_size = 0;

    /* invalidate? offset starts at 1, no value for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* ensure powers of two are available up to n (1‑based index) */
    if (pow2_size < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(pow2_arr, (new_size + 1) * sizeof(seq_t));
        if (!tmp && new_size > 0)
            exit(1);                      /* allocation failure */
        pow2_arr = tmp;
        for (idx_t i = pow2_size + 1; i <= new_size; ++i)
            pow2_arr[i] = pow2(i);
        pow2_size = new_size;
    }

    /* if we already have the value, return it */
    if (cache_size >= n)
        return cache[n - 1];

    /* initialise the cache if needed (a(1) = 1) */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        cache_size = 1;
    }

    /* extend the cache to contain a(n) */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache)
        exit(1);
    cache = new_cache;

    /* compute any missing terms up to n */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t prod = (seq_t)1;
        seq_t pow2_k = pow2_arr[k];
        for (idx_t i = 1; i < k; ++i) {
            seq_t term = pow2_arr[i] + pow2_k - ((seq_t)2);
            prod = prod * term;
        }
        cache[k - 1] = cache[k - 2] * prod;   /* a(k) = a(k‑1) · product_{i<k} (…) */
    }

    cache_size = n;
    return cache[n - 1];
}
