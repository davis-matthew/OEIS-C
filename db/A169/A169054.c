/*
 * A169054  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0) = 1, a(n) = 2 +
 *            39·(a(n‑1)+…+a(n‑23)) – 780·a(n‑24) for n ≥ 1, with a(k)=0 for
 *            k<0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;   /* sequence cache */
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_computed = 0; /* next index to compute */

/* Ensure the cache is allocated for indices 0 … n */
static void ensure_cache_cap(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = seq_alloc_array(new_cap);
    for (idx_t i = 0; i < cache_computed; ++i) new_cache[i] = cache[i];
    seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    if (n >= cache_computed) {
        ensure_cache_cap(n);
        for (idx_t i = cache_computed; i <= n; ++i) {
            seq_t termN = 0;
            if (i == 0)      termN = 1;
            else if (i <= 23) termN = 2;
            else if (i == 24) termN = 1;   /* N[24] = 1 */
            seq_t val = termN;

            /* add 39·a(i‑j) for 1 ≤ j ≤ min(i,23) */
            for (size_t j = 1; j <= 23 && j <= (size_t)i; ++j)
                val += cache[i - j] * (seq_t)39;

            /* subtract 780·a(i‑24) if i ≥ 24 */
            if (i >= 24)
                val -= cache[i - 24] * (seq_t)780;

            cache[i] = val;
        }
        cache_computed = n + 1;
    }
    return cache[n];
}
