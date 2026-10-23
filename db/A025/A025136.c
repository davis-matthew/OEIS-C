/*
 * A025136  a(n) = 2nd elementary symmetric function of C(n,0), C(n,1), ...,
 *          C(n,[ n/2 ]).
 * https://oeis.org/A025136
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   compute_a_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A025136
 *
 *  a(n) = Σ_{0 ≤ i < j ≤ ⌊n/2⌋} C(n,i)·C(n,j)
 *        where C(n,k) is the binomial coefficient.
 *
 *  This is the second elementary symmetric sum of the first ⌊n/2⌋+1
 *  binomial coefficients of n.
 *
 *  The implementation computes the binomial coefficients
 *  iteratively and uses the identity
 *
 *      a(n) = ((Σ C)² – Σ C²) / 2
 *
 *  for O(n) time and O(n) memory per call.  A simple cache is used
 *  so that repeated queries upscale only as needed.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/compute_a_for.h"

/* Cached values of the sequence.  They are grown only as needed. */
static seq_t *cache = NULL;
static size_t cache_size = 0;     /* allocated capacity */
static idx_t  max_calculated = 0; /* highest n already computed */

/* Ensure that cache holds values up to at least index n. */
static void ensure_cache(idx_t n)
{
    if (n < 2) return;            /* nothing to do for n < 2 */

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL)
    {
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    }
    else if ((size_t)(n + 1) > cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return; /* allocation failed – leave old cache */
        cache     = tmp;
        cache_size = (size_t)(n + 1);
    }

    /* Compute any new values that have not yet been cached. */
    for (idx_t k = max_calculated + 1; k <= n; ++k)
        cache[k] = compute_a_for(k);

    max_calculated = n;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* defined only for n ≥ 2 */

    ensure_cache(n);
    return cache[n];
}
