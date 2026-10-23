/*
 * A169422  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter *
 *            group on 25 generators S_i with relations S_i^2 = (S_i S_j)^32 =
 *            I. * * The generating function is * (t^32 + 2*t^31 + ... + 2*t +
 *            1) / * (1 - 23*(t + t^2 + ... + t^31) + 276*t^32) * * From this
 *            the sequence satisfies the linear recurrenc...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy, memset */

/* helper: compute the first 32 terms by polynomial division */
static void
compute_initial_terms(seq_t *init)
{
    const idx_t INIT = 32;
    seq_t running = 0;      /* running sum of previous terms */

    /* a[0] = 1 */
    init[0] = (seq_t)1;
    running = init[0];

    /* for n = 1 … 31: a[n] = 2 + 23·sum_{i=0}^{n-1} a[i] */
    for (idx_t n = 1; n < INIT; ++n)
    {
        init[n] = (seq_t)2 + ((seq_t)23) * running;
        running += init[n];
    }
}

/* required OEIS function */
seq_t
a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache       = NULL;
    static idx_t cache_size   = 0;
    static const idx_t INIT   = 32;          /* first 32 terms are precomputed */

    /* enlarge cache if necessary (and make sure it can hold the first 32 terms) */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        if (new_size < INIT)
            new_size = INIT;

        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;    /* memory failure – not expected */

        if (cache)
        {
            /* copy old data into the larger buffer */
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;

        /* initialise the first 32 terms (overwrites if they were already there) */
        compute_initial_terms(cache);
    }

    /* compute terms beyond the ones already present (if any) */
    for (idx_t i = INIT; i <= n; ++i)
    {
        /* i >= 32 always true in this loop */
        seq_t sum = 0;

        /* 23 · (a[i-1] + … + a[i-31]) */
        for (idx_t j = 1; j <= 31; ++j)
            sum += cache[i - j] * ((seq_t)23);

        /* – 276 · a[i-32] */
        sum -= cache[i - 32] * ((seq_t)276);

        /* inhomogeneous part for n = 32 */
        if (i == 32)
            sum += (seq_t)1;

        cache[i] = sum;
    }

    return cache[n];
}
