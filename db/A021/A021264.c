/*
 * A021264  Expansion of 1/((1-x)(1-2x)(1-8x)(1-9x)).
 * https://oeis.org/A021264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/((1‑x)(1‑2x)(1‑8x)(1‑9x)) Recurrence (valid for n ≥ 4):
 *            a(0) = 1, a(1) = 20, a(2) = 275, a(3) = 3250 a(n) = 20*a(n‑1) –
 *            125*a(n‑2) + 250*a(n‑3) – 144*a(n‑4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Dynamic cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;   /* allocated slots */
static idx_t cache_filled = 0;      /* indices 0 .. cache_filled‑1 are valid */

/* Expand the cache array so that it can hold at least 'min_cap' elements. */
static void grow_cache(size_t min_cap)
{
    if (cache_capacity >= min_cap)
        return;

    size_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap < min_cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failure */

    /* Copy existing terms */
    for (size_t i = 0; i < cache_filled; ++i)
        tmp[i] = cache[i];

    /* Free old storage */
    if (cache)
        free(cache);

    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute sequence terms up to index n, filling the cache as needed. */
static void compute_upto(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx < cache_filled)
        return;                  /* already computed */

    grow_cache(idx + 1);

    for (size_t i = cache_filled; i <= idx; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)20;
        } else if (i == 2) {
            cache[2] = (seq_t)275;
        } else if (i == 3) {
            cache[3] = (seq_t)3250;
        } else {
            /* a(n) = 20*a(n‑1) – 125*a(n‑2) + 250*a(n‑3) – 144*a(n‑4) */
            cache[i] = (seq_t)((seq_t)20 * cache[i-1]
                               - (seq_t)125 * cache[i-2]
                               + (seq_t)250 * cache[i-3]
                               - (seq_t)144 * cache[i-4]);
        }
    }

    cache_filled = idx + 1;
}

seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[(size_t)n];
}
