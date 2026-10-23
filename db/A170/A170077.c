/*
 * A170077  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18 * sum_{k=1}^{36} a(n-k) - 171 * a(n-37) + f(n) where
 *            f(0)=1, f(1..36)=2, f(37)=1, f(n>37)=0 (OEIS A170077)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* stores already computed a(0)…a(max) */
static idx_t cache_size = 0;  /* number of elements currently stored */

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) { abort(); }          /* out‑of‑memory; terminate */
    cache = new_cache;

    /* compute new entries from cache_size up to n inclusive */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* sum of the previous 36 terms a(i-1) … a(i-36) */
        for (int k = 1; k <= 36 && i >= k; ++k) {
            sum += cache[i - k];
        }

        seq_t term = 0;
        /* inhomogeneous part f(i) */
        if (i == 0) {
            term = 1;
        } else if (i <= 36) {
            term = 2;
        } else if (i == 37) {
            term = 1;
        } else {
            term = 0;
        }

        term += (seq_t)18 * sum;
        if (i >= 37) {
            term -= (seq_t)171 * cache[i - 37];
        }

        cache[i] = term;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
