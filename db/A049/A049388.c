/*
 * A049388  a(n) = (n+7)!/7!.
 * https://oeis.org/A049388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+7)! / 7! (equivalently, a(0)=1 and a(n)=a(n-1)*(n+7)
 *            for n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_capacity, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/seq_mul.h"

/* seq_t and idx_t are defined by the build system.  The function returns the
   nth term of OEIS A049388.  It memoizes results so repeated calls for any
   n are efficient. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* current allocated capacity (in elements)     */
    static idx_t  last   = 0;   /* highest index computed so far                */

    /* Initialise cache on the first call.  cache[0] = 1 according to the formula. */
    if (cache == NULL) {
        /* Allocate space for at least one element. */
        ensure_capacity(1, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last = 0;
    }

    /* If the requested n is already cached, return the stored value. */
    if (n <= last)
        return cache[n];

    /* Need to compute values for indices last+1 .. n.  Ensure the array is large enough. */
    if (n >= cap)
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));

    /* Compute the remaining values using the recurrence a(k) = a(k-1)*(k+7). */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t mult = (seq_t)(k + 7);                     /* small integer cast to seq_t */
        cache[k] = seq_mul(cache[k - 1], mult);          /* a(k) = a(k-1) * (k+7) */
    }

    last = n;  /* Update the highest computed index. */
    return cache[n];
}
