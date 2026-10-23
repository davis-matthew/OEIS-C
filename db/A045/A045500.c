/*
 * A045500  Fifth-from-right diagonal of triangle A121207.
 * https://oeis.org/A045500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   a(n+1) = Σ_{k=0..n} binomial(n+4, k+4) * a(k)  for n ≥ 0
   (equivalently, for i ≥ 1: a(i) = Σ_{k=0..i-1} binomial(i+3, k+4) * a(k))
*/
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Static memoisation */
    static seq_t *cache  = NULL;  /* array of already computed values   */
    static size_t cache_size = 0; /* allocated size (number of elements) */
    static idx_t last_computed = 0; /* highest index for which a[i] is known */

    /* Initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 1;                      /* hold a(0) */
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure – unlikely in tests */
        cache[0] = 1;                        /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow the cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* a(i) = Σ_{k=0..i-1} binomial(i+3, k+4) * a(k) */
        for (idx_t k = 0; k <= i - 1; ++k) {
            seq_t prod = binomial(i + 3, k + 4);
            prod *= cache[k];
            sum += prod;
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
