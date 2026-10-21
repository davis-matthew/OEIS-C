/*
 * A098460  Expansion of e.g.f. 1/sqrt(1-2x-2x^2).
 * https://oeis.org/A098460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(n) + (1-2n) a(n-1) - 2 (n-1)^2 a(n-2) = 0 with a(0)
 *            = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* cache = NULL;   /* memoization buffer              */
static size_t cache_cap = 0;  /* allocated capacity in elements   */

/* Ensure cache has at least need elements, zero‑initialized. */
static void ensure_cache(idx_t need)
{
    size_t new_cap = cache_cap;
    if (new_cap < need)
    {
        if (new_cap == 0) new_cap = 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);   /* out of memory */
        cache = tmp;
        /* Zero the new part */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    size_t need = (size_t)n + 1;
    ensure_cache(need);

    /* make sure base terms are stored (in case cache was freshly allocated) */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)1;

    /* compute missing terms up to n */
    for (size_t i = 2; i <= n; ++i)
    {
        if (cache[i] == 0)
        {
            seq_t a_prev   = cache[i - 1];
            seq_t a_prev2  = cache[i - 2];
            seq_t k = (seq_t)(i - 1);

            seq_t term1 = 2 * k * k * a_prev2;          /* 2 (i-1)^2 a(n-2) */
            seq_t term2 = ((seq_t)(2 * i) - 1) * a_prev;/* (2i-1) a(n-1)    */

            cache[i] = term1 + term2;
        }
    }
    return cache[n];
}
