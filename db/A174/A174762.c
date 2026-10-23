/*
 * A174762  x-values in the solution x^2-61*y^2=1.
 * https://oeis.org/A174762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3532638098*a(n-1) - a(n-2) (a(1)=1, a(2)=1766319049)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include <stdbool.h>               /* bool */

/* Utilities providing dynamic array management */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array()          */
#include "../utils/ensure_cache.h"      /* ensure_cache()             */

/* Return the n‑th term of A174762, 1‑based indexing.  Calls may come in any
 * order; the results are memoized in a static cache. */
seq_t a(idx_t n)
{
    /* Cache 1..computed.  Index 0 is unused. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;          /* allocated size (in entries) */
    static idx_t  computed = 0;           /* highest index actually stored */

    /* Undefined for n==0 or n<1: return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Initial allocation: 3 slots (0,1,2).  Store base values. */
    if (cache == NULL) {
        cache_cap = 3;
        cache = alloc_seq_array(cache_cap);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1766319049;
        computed = 2;
    }

    /* Extend cache if necessary.  Note: ensure_cache() guarantees that
     * the array can hold indices 0 .. (required_cap-1).  We ask for
     * at least (n+1) entries so that index n is valid. */
    if (n > computed) {
        ensure_cache((idx_t)(n + 1), &cache, &cache_cap);
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Recurrence in the native seq_t type.  The cast ensures that
             * the constant is treated as seq_t (wrap‑around allowed if
             * seq_t is too small). */
            cache[i] = ((seq_t)3532638098 * cache[i - 1]) - cache[i - 2];
        }
        computed = n;
    }

    return cache[n];
}
