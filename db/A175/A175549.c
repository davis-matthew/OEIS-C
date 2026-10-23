/*
 * A175549  Number of triples (a, b, c) with gcd(a, b, c) = 1 and -n <= a,b,c
 *          <= n.
 * https://oeis.org/A175549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 863, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175549: Number of triples (a,b,c) with gcd(a,b,c)=1 and -n<=a,b,c<=n
   Formula: for n > 0
       a(n) = 2*n*(4*n^2 + 6*n + 3) - Sum_{j=2..n} a(floor(n/j))
   (with a(0)=0, a(1)=26, … as given)
   The function memoises previous values so that calls in any order
   return immediately after the first pass. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache of computed a(i) */
static idx_t cache_size = 0;         /* size of cache (number of entries) */
static idx_t next_compute = 1;       /* next index that needs to be computed */

seq_t a(idx_t n)
{
    /* Allocate/expand the cache if necessary.  */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* memory failure (unlikely) */

        if (cache) {
            memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
            free(cache);
        }

        /* Zero‑initialise the new part of the cache.  */
        memset(new_cache + cache_size,
               0,
               (size_t)(n + 1 - cache_size) * sizeof(seq_t));

        cache      = new_cache;
        cache_size = n + 1;
    }

    /* Compute all a(i) up to n that have not yet been computed.  */
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t ii    = (seq_t)i;
        seq_t term  = (seq_t)2 * ii *
                     ((seq_t)4 * ii * ii + (seq_t)6 * ii + (seq_t)3);

        seq_t sum = 0;
        idx_t j = 2;
        while (j <= i) {
            idx_t t    = i / j;
            idx_t last = i / t;
            seq_t count = (seq_t)(last - j + 1);
            sum += cache[t] * count;
            j = last + 1;
        }
        cache[i] = term - sum;
    }

    next_compute = n + 1;
    return cache[n];
}
