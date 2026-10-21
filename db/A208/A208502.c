/*
 * A208502  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208502: Number of 4×n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
   and 0 1 0 and 1 1 1 vertically.  
   Recurrence:  a(n) = a(n-1) + 4*a(n-2)   for n > 4
   Initial values: a(1)=9, a(2)=81, a(3)=126, a(4)=324
   This implementation uses a simple memoised recurrence. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) for i >= 0      */
static idx_t cache_cap = 0;   /* number of entries currently cached  */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the offset but return 0 safely */
        return (seq_t)0;

    /* Return cached value if already computed */
    if (n <= cache_cap)
        return cache[n - 1];

    /* Increase the cache to store values up to n */
    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)            /* allocation failed – give up */
        return (seq_t)0;
    cache = tmp;

    /* Initialise base cases up to min(4,n) */
    if (cache_cap < 4) {
        const seq_t base[4] = { (seq_t)9, (seq_t)81, (seq_t)126, (seq_t)324 };
        for (idx_t i = 1; i <= 4 && i <= n; ++i)
            cache[i - 1] = base[i - 1];
    }

    /* Compute new terms using the recurrence */
    idx_t start = cache_cap + 1;   /* first index that needs computation */
    if (start < 5)                 /* ensure we start from a(5) at least */
        start = 5;

    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-2)  →  cache[i-1] = cache[i-2] + 4*cache[i-3] */
        cache[i - 1] = cache[i - 2] + (seq_t)4 * cache[i - 3];
    }

    cache_cap = n;                 /* update cache capacity              */
    return cache[n - 1];
}
