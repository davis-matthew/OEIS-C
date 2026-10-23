/*
 * A037665  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0.
 * https://oeis.org/A037665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-3) - 9*a(n-4); Initial values: a(1)=3,
 *            a(2)=28, a(3)=252, a(4)=2271 (For n<1 the value is undefined; we
 *            return 0 for safety.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <cstdlib>    /* for std::size_t, std::abort */
#include <algorithm>  /* for std::copy */
#else
#include <stdlib.h>   /* for malloc, realloc, abort */
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    /* Static cache of computed terms.  Indices start at 1. */
    static seq_t *cache = NULL;
    static idx_t cap     = 0;   /* allocated array size */
    static idx_t computed = 0;  /* highest index that has been computed */

    /* Ensure enough storage for indices up to n. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 5;   /* a few spare slots */
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            std::copy(cache, cache + cap, tmp);
            delete[] cache;
        }
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failure */
#endif
        cache = tmp;
        cap   = new_cap;

        /* Initialise the first four terms if they have not yet been set. */
        if (computed == 0) {
            cache[1] = (seq_t)3;
            cache[2] = (seq_t)28;
            cache[3] = (seq_t)252;
            cache[4] = (seq_t)2271;
            computed = 4;
        }
    }

    /* If the requested term has already been computed, just fetch it. */
    if (n <= computed) {
        return cache[n];
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 4) {
            /* Missing initial terms should already have been set; this
               block is only a safeguard if the function is called with
               a very small n before the cache was initialised. */
            switch (i) {
                case 1: cache[1] = (seq_t)3;   break;
                case 2: cache[2] = (seq_t)28;  break;
                case 3: cache[3] = (seq_t)252; break;
                case 4: cache[4] = (seq_t)2271;break;
            }
        } else {
            /* Recurrence: a(i) = 9*a(i-1) + a(i-3) - 9*a(i-4) */
            seq_t term = cache[i-1] * ((seq_t)9)
                       + cache[i-3]
                       - cache[i-4] * ((seq_t)9);
            cache[i] = term;
        }
    }
    computed = n;

    return cache[n];
}
