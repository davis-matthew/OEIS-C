/*
 * A102882  Expansion of (1+2x)/sqrt((1-3x^2)(1+4x+5x^2)).
 * https://oeis.org/A102882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   Recurrence for n≥4: n*(n+1)*a(n) + 2*(2*n+3)*(n-1)*a(n-1) +
 *            2*(n²-7)*a(n-2) + 6*(5+n-2*n²)*a(n-3) - 15*(n+2)*(n-3)*a(n-4) =
 *            0 Initial terms: a(0)=1, a(1)=0, a(2)=1, a(3)=2
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 23, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_size = 0;     /* allocated size of cache */
static idx_t cached_up_to = 0;   /* highest index that has a valid value */

/* enlarge cache if required and initialise the first four terms */
static void extend_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;              /* allocation failed – silently abort */

    /* copy existing cached values */
    for (idx_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];

    /* initialise new slots to 0 before we overwrite the first four terms */
    for (idx_t i = old_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    free(cache);
    cache = new_cache;
    cache_size = new_size;

    /* ensure the first four terms are present whenever the array is big
       enough.  If they are already defined (because we resized from a
       larger array) this simply overwrites the same values. */
    if (new_size > 0) cache[0] = (seq_t)1;
    if (new_size > 1) cache[1] = (seq_t)0;
    if (new_size > 2) cache[2] = (seq_t)1;
    if (new_size > 3) cache[3] = (seq_t)2;

    /* Maintain cached_up_to correctly:
       • if we just created a cache small enough to hold a few initial
         terms, set it to the highest existing index.
       • when the cache grows to include index 3, set cached_up_to to 3.
       • otherwise keep the previous value (which may already be ≥ 3). */
    if (old_size < 4 && new_size >= 4) {
        cached_up_to = 3;
    } else if (new_size < 4) {
        cached_up_to = new_size - 1;
    }
    /* for the remaining case (old_size≥4, new_size≥4) nothing changes
       and cached_up_to stays at its previous value. */
}

/* compute all terms up to n using the recurrence */
static void compute_terms(idx_t n)
{
    if (n <= cached_up_to) return;          /* already known */
    for (idx_t k = cached_up_to + 1; k <= n; ++k) {
        seq_t ks = (seq_t)k;

        seq_t term1 = 2 * (2 * ks + 3) * (ks - 1) * cache[k - 1];
        seq_t term2 = 2 * ((ks * ks) - 7) * cache[k - 2];
        seq_t term3 = 6 * (5 + ks - 2 * (ks * ks)) * cache[k - 3];
        seq_t term4 = 15 * (ks + 2) * (ks - 3) * cache[k - 4];

        seq_t numerator   = -term1 - term2 - term3 + term4;
        seq_t denominator = ks * (ks + 1);

        cache[k] = numerator / denominator;
    }
    cached_up_to = n;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    compute_terms(n);
    return cache[n];
}
