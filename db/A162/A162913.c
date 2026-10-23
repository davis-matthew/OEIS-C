/*
 * A162913  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=48, a(2)=2256, a(3)=104904, a(n) = 46*a(n‑1) +
 *            46*a(n‑2) – 1081*a(n‑3) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;   /* static cache of computed values */
static size_t cache_cap = 0;  /* currently allocated capacity */

/* enlarge cache to at least n+1 entries */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap == 0 ? 4 : cache_cap);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

        if (cache) seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* make sure cache is big enough */
    ensure_cache(n);

    /* initialise the first four terms once */
    if (cache[0] == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)48;
        cache[2] = (seq_t)2256;
        cache[3] = (seq_t)104904;
    }

    if (n < 4) return cache[(size_t)n];

    /* compute missing values up to n */
    for (size_t i = 4; i <= (size_t)n; ++i) {
        seq_t t = (seq_t)46 * cache[i-1];
        t += (seq_t)46 * cache[i-2];
        t -= (seq_t)1081 * cache[i-3];
        cache[i] = t;
    }
    return cache[(size_t)n];
}
