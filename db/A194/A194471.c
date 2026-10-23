/*
 * A194471  E.g.f. A(x) satisfies A(x) = exp(x) + x*A(x)^2.
 * https://oeis.org/A194471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n * Sum_{k=0..n-1} binom(n-1,k) * a(k) * a(n-1-k)
 *            Offset: 0 Computed by memoized recurrence, using auxiliary
 *            functions from utils
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

/* Cache for already computed terms */
static seq_t *cache = NULL;        /* pointer to array of a(n) */
static size_t cache_size = 0;      /* number of slots allocated */
static idx_t  cache_next = 0;      /* first index not yet computed */

/* Ensure the cache is large enough to hold indices up to idx (inclusive). */
static void
ensure_cache(idx_t idx)
{
    if ((size_t)idx < cache_size)
        return;                       /* already large enough */

    size_t new_cap = (size_t)idx + 1;
    seq_t *tmp = seq_realloc(cache, cache_size, new_cap);
    if (!tmp)
        abort();                      /* allocation failure */

    /* Initialize new slots to 0 */
    for (size_t i = cache_size; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_size = new_cap;
}

/* Compute a(n) with memoisation */
seq_t
a(idx_t n)
{
    ensure_cache(n);

    /* Initialise a(0) on first use */
    if (cache_next == 0) {
        cache[0] = 1;            /* a(0) = 1 */
        cache_next = 1;          /* next index to compute */
    }

    /* If the requested value is already computed, return it immediately */
    if (n < cache_next)
        return cache[n];

    /* Compute missing terms up to n inclusive */
    for (idx_t i = cache_next; i <= n; ++i) {
        if (i == 0) {            /* a(0) already set */
            continue;
        }

        idx_t m = i - 1;
        seq_t sum = 0;
        seq_t binom = 1;                      /* C(m,0) */
        idx_t half = m >> 1;                 /* floor((m)/2) */

        for (idx_t k = 0; k <= half; ++k) {
            idx_t j = m - k;

            seq_t prod = binom * cache[k];
            prod *= cache[j];
            if (k < j)            /* double due to symmetry (k != j) */
                prod += prod;

            sum += prod;

            /* Compute next binomial coefficient for next iteration */
            if (k < half) {
                binom *= (seq_t)(m - k);
                binom /= (seq_t)(k + 1);
            }
        }

        cache[i] = (seq_t)i * sum + 1;   /* a(i) = i*sum + 1 */
    }

    cache_next = n + 1;      /* update next uncomputed index */
    return cache[n];
}
