/*
 * A020880  Number of strong elementary edge-subgraphs in Moebius ladder M_n.
 * https://oeis.org/A020880
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 9*a(n-3) - 8*a(n-4) + 12*a(n-5) +
 *            5*a(n-6) - 6*a(n-7) - a(n-8) + a(n-9) (for n>10) Initial values:
 *            a(2)=6, a(3)=21, a(4)=26, a(5)=81, a(6)=129, a(7)=349, a(8)=650,
 *            a(9)=1614, a(10)=3281
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* only if needed; plain malloc works as well */

/* Static cache for memoised values */
static seq_t *cache = NULL;   /* allocated with size >= last_n+1  */
static idx_t   last_n = 0;    /* largest n that has been computed */

/* Ensure cache is large enough and initialize base values if freshly allocated */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (n + 1 > last_n)
    {
        /* Resize */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();  /* out of memory */
        cache = new_cache;
        /* If newly allocated, initialise base terms when first time */
        if (last_n == 0)
        {
            /* Undefined indices 0 and 1 are zero */
            cache[0] = 0;
            cache[1] = 0;
            /* Base values 2..10 */
            cache[2] = 6;
            cache[3] = 21;
            cache[4] = 26;
            cache[5] = 81;
            cache[6] = 129;
            cache[7] = 349;
            cache[8] = 650;
            cache[9] = 1614;
            cache[10] = 3281;
            last_n = 10;
        }
        /* Compute any missing values up to n */
        for (idx_t i = last_n + 1; i <= n; ++i)
        {
            /* Positive contributions */
            seq_t sum_pos =
                (seq_t)2 * cache[i - 1] +
                (seq_t)5 * cache[i - 2] +
                (seq_t)12 * cache[i - 5] +
                (seq_t)5 * cache[i - 6] +
                cache[i - 9];
            /* Negative contributions */
            seq_t sum_neg =
                (seq_t)9 * cache[i - 3] +
                (seq_t)8 * cache[i - 4] +
                (seq_t)6 * cache[i - 7] +
                cache[i - 8];
            cache[i] = sum_pos - sum_neg;
        }
        last_n = n;
    }
}

/* Public function expected by the harness */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined for n < 2, return 0 */
    if (n <= 10)                     /* base values already set */
    {
        if (!cache) ensure_cache(10);
        return cache[n];
    }
    ensure_cache(n);                /* compute up to n if necessary */
    return cache[n];
}
