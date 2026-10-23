/*
 * A151863  a(0)=1; a(1)=0; a(2)=2; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1; a(1)=0; a(2)=2;
   for n≥3 let n=2^i + j (0≤j<2^i):
       a(n)=2*a(j)+a(j+1) (+1 if j=2^i‑1). */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values          */
static size_t cache_cap = 0;     /* allocated size of cache           */
static size_t next_index = 0;    /* next index to compute (i.e. first
                                   unused position in cache)         */

/* Ensure the cache holds at least indices 0..n, inclusive. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (cache_cap >= required) return;
    seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = required;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Compute values for all indices that are still unknown. */
    for (idx_t i = (idx_t)next_index; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)2;
        } else {
            /* largest power of two <= i */
            idx_t p = 1;
            while ((p << 1) <= i) p <<= 1;
            idx_t j = i - p;                    /* 0 ≤ j < p */
            seq_t val = 2 * cache[j] + cache[j + 1];
            if (j == p - 1) ++val;              /* extra 1 when j == 2^i‑1 */
            cache[i] = val;
        }
    }
    next_index = (size_t)n + 1;
    return cache[n];
}
