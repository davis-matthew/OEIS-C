/*
 * A168842  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies a(0)=1, a(1)=21, a(i)=420·20^(i‑2) for
 *            2≤i≤19, and for i≥20: 10·a(i) = a(i‑1)+…+a(i‑20)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache has at least n+1 entries; extend if needed. */
static void extend_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;

    /* if not initialized, start with capacity 20 */
    if (cache_cap == 0) {
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* resize if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise new entries */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1;
        } else if (k == 1) {
            val = (seq_t)21;
        } else if (k <= 19) {
            /* val = 420 * 20^(k-2) */
            val = (seq_t)420;
            for (idx_t j = 2; j < k; ++j)
                val *= (seq_t)20;
        } else {
            /* use recurrence: a(k) = (a(k-1)+...+a(k-20)) / 10 */
            seq_t sum = 0;
            for (idx_t j = k - 20; j < k; ++j)
                sum += cache[j];
            val = sum / (seq_t)10;
        }
        cache[k] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
