/*
 * A124861  Expansion of 1/(1-x-3*x^2-4*x^3-2*x^4).
 * https://oeis.org/A124861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 4*a(n-3) + 2*a(n-4) with initial
 *            values a(0)=1, a(1)=1, a(2)=4, a(3)=11 (OEIS A124861)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_seq, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"

/* Compute a(n).  The function is memoised so that repeated
   calls with the same or larger n incur no extra cost. */
seq_t a(idx_t n)
{
    /* Static cache -------------------------------------------------- */
    static seq_t *cache = NULL;        /* array of already computed terms */
    static size_t cache_cap = 0;       /* allocated size of array        */
    static idx_t  cache_last = -1;     /* highest index computed so far  */

    /* Initialise cache with the first four terms if necessary */
    if (cache == NULL) {
        /* Allocate space for the first four terms */
        cache = alloc_seq(4);
        cache_cap = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache_last = 3;
    }

    /* If the requested n is already available, return it immediately */
    if ((idx_t)n <= cache_last)
        return cache[n];

    /* Resize the cache if it cannot contain the index n */
    size_t needed_cap = (size_t)n + 1;       /* we need indices 0..n inclusive */
    if (cache_cap < needed_cap)
        grow_seq(&cache, &cache_cap, needed_cap);

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 3*a(i-2) + 4*a(i-3) + 2*a(i-4) */
        cache[i] = cache[i-1]
                 + cache[i-2] * (seq_t)3
                 + cache[i-3] * (seq_t)4
                 + cache[i-4] * (seq_t)2;
    }

    /* Update cache metadata */
    cache_last = n;
    return cache[n];
}
