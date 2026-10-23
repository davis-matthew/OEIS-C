/*
 * A010910  Pisot sequence E(4,27): a(n) = floor(a(n-1)^2/a(n-2)+1/2) for n>1,
 *          a(0)=4, a(1)=27.
 * https://oeis.org/A010910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=4, a(1)=27, a(n)=6*a(n-1)+5*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;  /* array of computed terms      */
    static idx_t  last    = 0;     /* highest index computed so far */
    static idx_t  sz      = 0;     /* current allocation size      */

    /* Initialise the cache with the first two terms on first call. */
    if (cache == NULL) {
        sz = 2;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();         /* out of memory */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)27;
        last = 1;
    }

    /* If the requested value is already cached, return it. */
    if (n <= last)
        return cache[n];

    /* Resize the cache if necessary.  We enlarge it exactly to n+1. */
    if (n >= sz) {
        idx_t new_sz = n + 1;
        seq_t *p = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!p) abort();          /* out of memory */
        cache = p;
        sz = new_sz;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = ((seq_t)6 * cache[i - 1]) + ((seq_t)5 * cache[i - 2]);

    last = n;
    return cache[n];
}
