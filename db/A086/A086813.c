/*
 * A086813  a(1)=1 then a(n)= (1/2) *(5*a(n-1)+1) if a(n-1) is odd,
 *          a(n)=3/2*a(n-1) otherwise.
 * https://oeis.org/A086813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = (5*a(n-1)+1)/2 if a(n-1) is odd, a(n) =
 *            3*a(n-1)/2 if a(n-1) is even. (n ≥ 2)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of already computed terms   */
static idx_t cache_size = 0;  /* number of terms stored in cache  */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache is initialised with a(1) = 1. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;   /* a(1) */
        cache_size = 1;
    }

    /* Grow the cache up to the requested index if necessary. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* out‑of‑memory fallback */
        cache = tmp;

        /* Compute new terms using the recurrence.  Terms are stored
           in 0‑based index: cache[i-1] = a(i). */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];   /* a(i-1) */
            seq_t next;

            /* Odd / even check: prev & 1 works for any unsigned type. */
            if ((prev & (seq_t)1) != 0)
                next = (prev * (seq_t)5 + (seq_t)1) / (seq_t)2;   /* odd case */
            else
                next = (prev * (seq_t)3) / (seq_t)2;              /* even case */

            cache[i - 1] = next;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
