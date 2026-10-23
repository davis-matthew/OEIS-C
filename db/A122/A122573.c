/*
 * A122573  Expansion of x*(1 + x)*(1 - 3*x^2)/(1 - 4*x^2 + x^4).
 * https://oeis.org/A122573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 38, u64 n <= 70, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122573:  Expansion of x*(1 + x)*(1 - 3*x^2)/(1 - 4*x^2 + x^4)
   Recurrence (n ≥ 5): a(n) = 4*a(n-2) - a(n-4)
   Initial terms: a(1)=1, a(2)=1, a(3)=1, a(4)=1
   (These are the first four values in the b-file)
*/

#include <stdlib.h>     /* malloc, free */

/* Cache of computed terms, 1‑based index. */
static seq_t *cache = NULL;    /* array of size cache_len = current capacity */
static idx_t cache_len = 0;    /* number of elements allocated in cache (≥ n+1) */
static idx_t last_computed = 0;/* largest index whose value is guaranteed to be correct */

/* Ensure the cache can hold indices up to n (inclusive) and compute any
   missing values using the recurrence. */
static void ensure_cache(idx_t n)
{
    /* Grow the array if necessary. */
    if ((idx_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */

        /* Initialise the new buffer with zeroes. */
        for (idx_t i = 0; i < new_len; ++i) {
            new_cache[i] = 0;
        }

        /* Copy any old values. */
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;
        cache_len = (idx_t)new_len;

        /* Set the base terms if the cache is large enough. */
        if (cache_len > 1) cache[1] = (seq_t)1;
        if (cache_len > 2) cache[2] = (seq_t)1;
        if (cache_len > 3) cache[3] = (seq_t)1;
        if (cache_len > 4) cache[4] = (seq_t)1;
    }

    /* Compute missing terms from the next index after the last computed one. */
    idx_t start = (last_computed + 1 > 5) ? last_computed + 1 : 5;
    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 4*a(i-2) - a(i-4) */
        cache[i] = (seq_t)4 * cache[i - 2] - cache[i - 4];
    }
    if (n > last_computed) last_computed = n;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* outside the declared offset */
    ensure_cache(n);
    return cache[n];
}
