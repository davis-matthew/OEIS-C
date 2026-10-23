/*
 * A014535  Number of B-trees of order 3 with n leaves.
 * https://oeis.org/A014535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 55, u64 n <= 102,
 *            u128 n <= 196, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A014535
 * Number of B‑trees of order 3 with n leaves
 *
 * Recurrence (for n > 1):
 *   a(n) = Σ_{k=ceil(n/3)}^{floor(n/2)}  binomial(k, 3*k - n) * a(k)
 *
 * Initial values:
 *   a(0) = 0
 *   a(1) = 1
 *
 * The function uses memoised recursion with a dynamic cache.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <string.h>   /* memset */

#include "../utils/binomial.h" /* seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements allocated */

    /* ensure cache can hold indices up to n */
    if ((idx_t)(n + 1) > cache_len) {
        idx_t new_len = n + 1;
        /* allocate new array */
        seq_t *new_cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed – unlikely */

        /* copy existing values (they are already in place) */
        if (cache) {
            memcpy(new_cache, cache, (size_t)cache_len * sizeof(seq_t));
            free(cache);
        }
        /* zero initialise the new portion */
        memset(new_cache + cache_len, 0, (size_t)(new_len - cache_len) * sizeof(seq_t));

        cache = new_cache;
        cache_len = new_len;
    }

    /* base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* if already computed, return */
    if (cache[n] != 0) return cache[n];

    /* compute a(n) by summing over k */
    idx_t lower = (n + 2) / 3;          /* ceil(n/3) */
    idx_t upper = n / 2;                /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = lower; k <= upper; ++k) {
        seq_t coeff = binomial(k, 3 * k - n);
        seq_t ak = a(k);                /* memoised recursive call */
        sum += coeff * ak;
    }

    cache[n] = sum;                     /* store for future calls */
    return sum;
}
