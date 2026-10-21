/*
 * A084267  Partial sums of a binomial quotient.
 * https://oeis.org/A084267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} ⌊ (k+3)(k+4) / 12 ⌋
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache to store already computed values when the function
   is called with arbitrary indices.  cache_length holds how many
   terms (starting from index 0) are in the cache. */
static seq_t *cache = NULL;
static size_t cache_length = 0;
static size_t cache_capacity = 0;

/* Ensure that the cache can hold at least [0 … need-1] indices. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_capacity = new_cap;
}

/* Compute a(n).  The function works for any n ≥ 0 and any ordering
   of calls.  It uses memoisation so repeated or out‑of‑order requests
   are handled efficiently. */
seq_t a(idx_t n)
{
    /* Make sure the cache has enough space. */
    ensure_cache_capacity((size_t)n + 1);

    /* If we already have the value, just return it. */
    if (cache_length > (size_t)n)
        return cache[(size_t)n];

    /* Compute missing terms incrementally. */
    for (idx_t k = cache_length; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t term = ((kk + (seq_t)3) * (kk + (seq_t)4)) / (seq_t)12;   /* floor division */

        if (k == 0)
            cache[0] = term;
        else
            cache[(size_t)k] = cache[(size_t)(k - 1)] + term;
    }

    cache_length = (size_t)n + 1;
    return cache[(size_t)n];
}
