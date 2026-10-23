/*
 * A170515  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 45: a(n) = 24 * Σ_{k=0}^{n-1} a(k) + 2.
 *            For n = 46: a(46) = 24 * Σ_{k=1}^{45} a(k) - 300 * a(0) + 1. For
 *            n ≥ 47: a(n) = 24 * Σ_{k=n-45}^{n-1} a(k) - 300 * a(n-46). This
 *            recurrence follows from the rational generating function given
 *            in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values */
static size_t cache_computed = 0;  /* number of elements currently stored (next free index) */
static size_t cache_alloc = 0;     /* current allocation size */

/* Grow the cache to hold at least needed_idx + 1 elements. */
static void ensure_cache(size_t needed_idx)
{
    if (needed_idx < cache_alloc)
        return;
    size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
    while (new_alloc <= needed_idx)
        new_alloc <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_alloc * sizeof(seq_t));
    if (cache) {
        for (size_t i = 0; i < cache_computed; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_alloc = new_alloc;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx);

    if (cache_computed <= idx) {
        for (size_t i = cache_computed; i <= idx; ++i) {
            seq_t term;
            if (i == 0) {
                term = (seq_t)1; /* a(0) = 1 */
            } else if (i <= 45) {
                seq_t sum = 0;
                for (size_t k = 0; k < i; ++k)
                    sum += cache[k];
                term = sum * (seq_t)24 + (seq_t)2;
            } else if (i == 46) {
                seq_t sum = 0;
                for (size_t k = 1; k <= 45; ++k)
                    sum += cache[k];
                term = sum * (seq_t)24 - cache[0] * (seq_t)300 + (seq_t)1;
            } else { /* i >= 47 */
                seq_t sum = 0;
                for (size_t k = i - 45; k < i; ++k)
                    sum += cache[k];
                term = sum * (seq_t)24 - cache[i - 46] * (seq_t)300;
            }
            cache[i] = term;
        }
        cache_computed = idx + 1;
    }
    return cache[idx];
}
