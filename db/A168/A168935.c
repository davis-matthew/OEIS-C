/*
 * A168935  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A168935 (Coxeter group on 18 generators):
   Let b(n) = 1 for n = 0 or n = 22, else b(n) = 2.
   For all n ≥ 0:
        a(n) = b(n) + Σ_{k=1}^{min(n,21)} 16·a(n–k) – (n≥22 ? 136·a(n–22) : 0).
   For n ≥ 22 this simplifies to the homogeneous recurrence
        a(n) = 16·(a(n–1)+…+a(n–21)) – 136·a(n–22). */

/*  Include the allocation routine for the type seq_t.  */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute terms a(0) … a(n) and store them in cache. */
static void ensure_terms(idx_t n)
{
    if (n < cache_len)
        return;                      /* already available */

    size_t new_len = (size_t)n + 1;
    seq_t *new_arr = alloc_seq_array(new_len);
    if (cache_len > 0)
        for (idx_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];

    const seq_t c16 = (seq_t)16;
    const seq_t c136 = (seq_t)136;

    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;

        /* add 16*a(i-k) for k = 1 … min(i,21) */
        idx_t k_max = i < 21 ? i : 21;
        for (idx_t k = 1; k <= k_max; ++k)
            sum += (cache[i - k] * c16);

        /* subtract 136*a(i-22) if i ≥ 22 */
        if (i >= 22)
            sum -= (cache[i - 22] * c136);

        /* b(i) = 1 if i==0 or i==22, else 2 */
        seq_t bi = (i == 0 || i == 22) ? (seq_t)1 : (seq_t)2;

        new_arr[i] = bi + sum;
    }

    /* free old cache */
    if (cache)
        free(cache);      /* safe for primitive types; adapted builds use this routine */

    cache     = new_arr;
    cache_len = new_len;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
