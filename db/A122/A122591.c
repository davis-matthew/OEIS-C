/*
 * A122591  a(n) = 2*a(n-1) - a(n-2) + (a(n-1)^2 + a(n-2)^2).
 * https://oeis.org/A122591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + (a(n-1)^2 + a(n-2)^2), a(1) = a(2) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A122591 with memoisation.
   The implementation works both for C (where seq_t is a builtin
   integral type) and for C++ (where seq_t may be an arbitrary
   precision integer class).  */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed terms;
                                       cache[0] is unused */
    static idx_t last   = 2;         /* highest index computed so far */
    static size_t cap   = 0;         /* allocated size of cache */

    /* If n < 1 the sequence is not defined; return 0 as safety. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the cache has room for terms up to index n. */
    if ((size_t)n + 1 > cap) {
        size_t need = (size_t)n + 1;
        seq_t *newp = NULL;

#if defined(__cplusplus)
        /* Allocate a larger array and copy existing data. */
        newp = new seq_t[need];
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                newp[i] = cache[i];
            delete[] cache;
        }
#else
        /* C: use malloc/division */
        newp = (seq_t*)malloc(need * sizeof(seq_t));
        if (!newp)
            abort(); /* out of memory */
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                newp[i] = cache[i];
            free(cache);
        }
#endif
        cache = newp;
        cap   = need;
    }

    /* Initialise the base cases each call.  They are small enough
       that we can recompute them safely. */
    cache[1] = (seq_t)1;
    if (n >= 2)
        cache[2] = (seq_t)1;

    /* If we already have the requested term, return it. */
    if (n <= last)
        return cache[n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        cache[i] = 2 * a1 - a2 + (a1 * a1 + a2 * a2);
    }

    last = n;
    return cache[n];
}
