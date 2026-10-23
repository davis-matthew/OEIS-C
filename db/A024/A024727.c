/*
 * A024727  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(2)=1 and a(3)=4.
 * https://oeis.org/A024727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *            initial values a(1)=a(2)=1, a(3)=4. (OEIS offset: 1)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of computed terms.
   cache[0] is unused so that a(n) can be accessed directly as cache[n]. */
static seq_t *cache      = NULL;   /* pointer to the cache array       */
static idx_t cache_cap   = 0;      /* allocated size (number of elements) */
static idx_t cache_max   = 0;      /* highest index whose value is known */

/* Ensure the cache is large enough to hold indices up to n (inclusive). */
static void expand_cache(idx_t n)
{
    /* Nothing to do if already big enough. */
    if (n + 1 <= cache_cap)
        return;

    /* New capacity is n+1.  Reallocate (or allocate if this is the first time). */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory – the test harness cannot recover */

    /* If the array grew, zero the new cells (uninitialized). */
    if (cache_cap < n + 1) {
        for (idx_t i = cache_cap; i <= n; ++i)
            new_cache[i] = (seq_t)0;
    }

    cache      = new_cache;
    cache_cap  = n + 1;
    /* cache_max remains unchanged; new cells will be filled by compute_up_to. */
}

/* Compute all terms up to and including n, storing them in the cache. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max)
        return;                 /* already known */

    /* Grow the cache if necessary. */
    expand_cache(n);

    /* Compute terms in increasing order. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k <= 3) {            /* initial values  */
            if (k == 1 || k == 2)
                cache[k] = (seq_t)1;
            else                 /* k == 3 */
                cache[k] = (seq_t)4;
        } else {
            seq_t sum = (seq_t)0;
            idx_t limit = (k + 1) / 4;                /* floor((k+1)/4) */
            for (idx_t i = 1; i <= limit; ++i) {
                idx_t odd   = 2 * i - 1;
                idx_t rest  = k - 2 * i + 1;
                sum += cache[odd] * cache[rest];
            }
            cache[k] = sum;
        }
    }

    cache_max = n;
}

/* External entry point. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined at n=0 – safe fallback */

    compute_up_to(n);
    return cache[n];
}
