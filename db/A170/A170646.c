/*
 * A170646  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=13, a(2)=156, ..., and for all n≥0 a(n) =
 *            11*(a(n‑1)+…+a(n‑48)) – 66*a(n‑49) + q_n where q_n = 1 if n==0
 *            or n==49, q_n = 2 if 1≤n≤48, else q_n = 0. This follows from the
 *            rational generating function in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;   /* Cache of computed terms */
static idx_t cache_len = 0;   /* Number of terms in cache */

/* Extend the cache so that it contains terms 0 … n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    idx_t new_len = n + 1;
    seq_t *new_cache = seq_alloc_array((size_t)new_len); /* may return NULL on failure */

    /* Copy already computed terms */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;

        /* 11 * (a(i-1)+…+a(i-48)) */
        for (idx_t j = 1; j <= 48 && i >= j; ++j)
            val += 11 * new_cache[i - j];

        /* –66 * a(i-49) */
        if (i >= 49)
            val -= 66 * new_cache[i - 49];

        /* Add q_i from the numerator */
        if (i == 0 || i == 49)
            val += 1;
        else if (i <= 48)
            val += 2;     /* q_i = 2 for 1 ≤ i ≤ 48 */

        new_cache[i] = val;
    }

    /* Replace old cache */
    free_seq_array(cache);
    cache      = new_cache;
    cache_len  = new_len;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
