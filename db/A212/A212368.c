/*
 * A212368  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 9).
 * https://oeis.org/A212368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ_{k=1}^{n-9} a(k) * a(n-9-k) with a(0) = 1,
 *            a(n) = 1 for 0 ≤ n ≤ 9
 * Range:     u8 n <= 23, u16 n <= 39, u32 n <= 69, u64 n <= 129,
 *            u128 n <= 246, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* memoized a(i) values                */
static idx_t  cap     = 0;      /* allocated size of cache             */
static idx_t  next    = 0;      /* smallest index not yet computed     */

/* Compute the n-th term of OEIS A212368 */
seq_t a(idx_t n)
{
    /* first call: allocate cache and seed known values */
    if (cache == NULL) {
        cap = (n > 9 ? n + 1 : 10);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* out of memory */

        /* values 0 … 9 are 1 */
        for (idx_t i = 0; i <= 9 && i < cap; ++i)
            cache[i] = (seq_t)1;
        /* initialise rest of array to zero */
        for (idx_t i = 10; i < cap; ++i)
            cache[i] = (seq_t)0;

        next = 10;                                   /* a(0)…a(9) done */
    }

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* out of memory */
        cache = tmp;
        cap = new_cap;
        /* initialise new slots (they are beyond the current `next`) */
        for (idx_t i = next; i < cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Return cached value if already computed */
    if (n < next)
        return cache[n];

    /* Compute all values from `next` up to `n` inclusive */
    for (idx_t i = next; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            idx_t limit = i - 9;
            for (idx_t k = 1; k <= limit; ++k)
                sum += cache[k] * cache[i - 9 - k];
            cache[i] = cache[i - 1] + sum;
        }
    }

    next = n + 1;          /* next index to compute */
    return cache[n];
}
