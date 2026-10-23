/*
 * A124217  Expansion of (1-x-x^2)/(1-2x-x^2+x^4).
 * https://oeis.org/A124217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124217 – Expansion of (1‑x‑x²)/(1‑2x‑x²+x⁴)  
   Recurrence (index 0‑based):  
   a(0)=1, a(1)=1, a(2)=2, a(3)=5;  
   a(n)=2*a(n‑1)+a(n‑2)−a(n‑4)   for n≥4   */

#include <stdlib.h>

/* Cache of already computed terms – grows on demand. */
static seq_t *seq_cache = NULL;      /* buffer of cached values          */
static size_t cache_cap = 0;         /* number of elements currently stored */

/* Base values for the first four terms. */
static const seq_t base_vals[4] = {1, 1, 2, 5};

/* Ensure that seq_cache contains terms up to (and including) n. */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)   /* nothing to do */
        return;

    size_t new_cap = (size_t)n + 1;

    /* realloc may be used with a NULL pointer; cast is needed for C++. */
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;

    /* Initialize missing base values if the cache was previously too small. */
    if (cache_cap < 4) {
        size_t start = cache_cap;          /* first uninitialised index */
        if (start > 3) start = 4;          /* already have all four */
        for (size_t i = start; i <= 3 && i < new_cap; ++i)
            seq_cache[i] = base_vals[i];
    }

    /* Compute additional terms using the recurrence. */
    size_t start = (cache_cap < 4) ? 4 : cache_cap;
    for (size_t i = start; i < new_cap; ++i) {
        seq_cache[i] = 2 * seq_cache[i - 1]
                     +   seq_cache[i - 2]
                     -   seq_cache[i - 4];
    }

    cache_cap = new_cap;
}

/* Public function: a(idx_t n) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
