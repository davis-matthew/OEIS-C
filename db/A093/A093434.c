/*
 * A093434  a(n) = Product_{i=1..n} (2*n-i)*(2*n+i).
 * https://oeis.org/A093434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (2*n-i)*(2*n+i) Recurrence: a(1) = 3,
 *            a(n+1) = 3*(3*n+2)*(3*n+1)*a(n) for n ≥ 1 Indexed with offset 1
 *            (a(1) is the first term)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[n] stores a(n) for n ≥ 1   */
static idx_t cache_cap = 0;      /* current capacity of cache array */
static idx_t cached_n  = 0;      /* largest n already computed       */

static void grow_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
#ifdef __cplusplus
        /* Allocate new array and copy existing elements */
        seq_t *new_cache = new seq_t[n + 1];
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        /* Grow using realloc (works for plain integral types) */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();              /* out‑of‑memory: terminate */
        cache = tmp;
#endif
        cache_cap = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence is defined only for n ≥ 1; return 0 for safety. */
        return (seq_t)0;
    }

    grow_cache(n);

    /* Compute any missing terms up to n. */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)3;
        } else {
            seq_t term = (seq_t)3 *
                         ((seq_t)(3 * (k - 1) + 2)) *
                         ((seq_t)(3 * (k - 1) + 1));
            cache[k] = cache[k - 1] * term;
        }
    }
    cached_n = n;

    return cache[n];
}
