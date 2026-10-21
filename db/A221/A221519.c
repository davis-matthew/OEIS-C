/*
 * A221519  Number of 0..3 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3) + a(n-4), with a(1)=0,
 *            a(2)=6, a(3)=10, a(4)=36. The recurrence is valid for n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to keep computed values across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (indices 0..cap-1) */
    static idx_t last_computed = 0;/* largest index computed so far */

    /* Allocate (or grow) the cache if necessary */
    if (cache == NULL) {
        /* first call: allocate enough space for indices 0..n */
        cache = (seq_t *)malloc((size_t)(n + 1u) * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure – unlikely */
        cache_cap = (size_t)(n + 1u);

        /* initialise the first four terms (if they exist) */
        if (n >= 1) cache[1] = (seq_t)0;
        if (n >= 2) cache[2] = (seq_t)6;
        if (n >= 3) cache[3] = (seq_t)10;
        if (n >= 4) cache[4] = (seq_t)36;

        /* record how many terms have been initialised */
        if (n >= 4) last_computed = 4;
        else if (n >= 3) last_computed = 3;
        else if (n >= 2) last_computed = 2;
        else if (n >= 1) last_computed = 1;
    }
    else if (cache_cap <= (size_t)n) {
        /* Need more space: reallocate to hold indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1u) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = (size_t)(n + 1u);

        /* If we had not yet initialised the first four terms,
           initialise them now.  (This happens only when the
           original allocation size was <5.) */
        if (last_computed < 4) {
            if (n >= 1) cache[1] = (seq_t)0;
            if (n >= 2) cache[2] = (seq_t)6;
            if (n >= 3) cache[3] = (seq_t)10;
            if (n >= 4) cache[4] = (seq_t)36;
            if (n >= 4) last_computed = 4;
            else if (n >= 3) last_computed = 3;
            else if (n >= 2) last_computed = 2;
            else if (n >= 1) last_computed = 1;
        }
    }

    /* Compute missing terms up to index n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* For i <= 4 the values are already known */
            if (i > 4) {
                cache[i] = cache[i-1]
                           + (seq_t)4 * cache[i-2]
                           + (seq_t)3 * cache[i-3]
                           + cache[i-4];
            }
        }
        last_computed = n;
    }

    return cache[n];
}
