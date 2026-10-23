/*
 * A041768  Numerators of continued fraction convergents to sqrt(405).
 * https://oeis.org/A041768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = 322*a(n-2)-a(n-4), with a(0)=20, a(1)=161, a(2)=6460,
 *            a(3)=51841
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* Sequential cache for the sequence terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_used = 0;  /* highest index computed + 1 */

/* Return the n-th term of OEIS A041768 (numerators of continued‐fraction
 * convergents to sqrt(405)).  Computes terms lazily and caches them
 * for repeated, out‑of‑order calls. */
seq_t a(idx_t n)
{
    /* Convert index to a size_t that is safe for array bounds. */
    size_t idx = (size_t)n;

    /* Increase cache capacity if necessary. */
    if (idx >= cache_cap)
    {
        size_t new_cap = idx + 1;
        if (cache == NULL)
        {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            cache_cap = new_cap;
            cache_used = 0;
        }
        else
        {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* If we already have the value, return it. */
    if (idx < cache_used)
        return cache[idx];

    /* Compute all missing values up to idx. */
    for (size_t i = cache_used; i <= idx; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)20;
        else if (i == 1)
            cache[i] = (seq_t)161;
        else if (i == 2)
            cache[i] = (seq_t)6460;
        else if (i == 3)
            cache[i] = (seq_t)51841;
        else
            cache[i] = ((seq_t)322) * cache[i - 2] - cache[i - 4];
    }

    cache_used = idx + 1;
    return cache[idx];
}
