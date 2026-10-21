/*
 * A205398  s(k)-s(j), where (k,j) is the least pair for which n divides
 *          s(k)-s(j), and s(j)=ceiling(j^2/2).
 * https://oeis.org/A205398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A205398
 *  Formula: a(n) = s(k) - s(j)
 *  where (k,j) is the least pair for which n divides s(k)-s(j),
 *  s(x) = ceiling(x^2 / 2).
 *  The algorithm finds the first repeated residue of s(k) modulo n
 *  and returns the difference of the corresponding s-values.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;   /* cache[n] holds a(n); cache[0] unused   */
static idx_t  cache_cap = 0;   /* current size of the cache (>= max n seen + 1) */

/* Compute a(n) for n >= 1.  For n == 0 a value of 0 is returned. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extend the cache if necessary. */
    if (n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;            /* +1 because we index by n */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;                             /* allocation failure */

        /* initialize the whole new cache to zero */
        for (size_t i = 0; i < new_cap; ++i)
            new_cache[i] = 0;

        /* copy old cache contents */
        if (cache)
        {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        cache      = new_cache;
        cache_cap  = (idx_t)new_cap;
    }

    /* Return memoized value if already computed. */
    if (cache[n] != 0)
        return cache[n];

    /* Array holding the first index k for each residue modulo n.  */
    /* 0 means “not yet seen”  (since indices start at 1).  */
    idx_t *first_idx = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first_idx)
        return 0;                                /* allocation failure */
    for (size_t i = 0; i < (size_t)n; ++i)
        first_idx[i] = 0;

    /* Find the first duplicate residue of s(k) modulo n. */
    for (idx_t k = 1; ; ++k)
    {
        /* Compute s(k) = ceil(k^2 / 2) as a seq_t value. */
        seq_t s_k = ( ( (seq_t)k * k ) + (seq_t)(k & 1) ) / 2;

        /* Residue of s(k) modulo n (use idx_t arithmetic). */
        idx_t r = ( ( (k * k) + (k & 1) ) / 2 ) % n;

        if (first_idx[r] == 0)          /* first occurrence of this residue */
        {
            first_idx[r] = k;
        }
        else                            /* repeated residue found */
        {
            idx_t j = first_idx[r];
            seq_t s_j = ( ( (seq_t)j * j ) + (seq_t)(j & 1) ) / 2;
            seq_t diff = s_k - s_j;
            cache[n] = diff;
            free(first_idx);
            return diff;
        }
    }
}
