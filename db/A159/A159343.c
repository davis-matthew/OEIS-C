/*
 * A159343  Transform of A056594 by the T_{0,1} transformation (see link).
 * https://oeis.org/A159343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3) - 2*a(n-4) + a(n-5) for
 *            n>=5, with a(0)=2, a(1)=3, a(2)=6, a(3)=16, a(4)=39
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cap   = 0;       /* current allocation size */
    static idx_t size  = 0;       /* number of terms stored (size==cap after first init) */

    /* Initialize cache with the first five terms. */
    if (cache == NULL) {
        cap   = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory – unlikely in tests */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)39;
        size = 5;
    }

    /* Grow the cache if the requested index is beyond what we have computed so far. */
    if (n >= size) {
        /* Determine a new capacity, at least the requested index + 1. */
        idx_t new_cap = cap;
        while (n >= new_cap) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* out of memory – unlikely in tests */
        cache = tmp;
        cap   = new_cap;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = size; i <= n; ++i) {
            seq_t ai = (seq_t)3 * cache[i-1]
                     - (seq_t)3 * cache[i-2]
                     + (seq_t)4 * cache[i-3]
                     - (seq_t)2 * cache[i-4]
                     +      cache[i-5];
            cache[i] = ai;
        }
        size = n + 1;
    }

    return cache[n];
}
