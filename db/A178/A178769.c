/*
 * A178769  a(n) = (5*10^n + 13)/9.
 * https://oeis.org/A178769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*10^n + 13)/9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=7, a(n)=10*a(n-1)-13 for n>1 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/grow_seq.h"          /* grow_seq */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last_index = 0;
    static bool   initialized = false;

    /* initialize cache on first call */
    if (!initialized) {
        /* allocate space for indices 0 and 1 */
        cap = 2;
        cache = alloc_seq_array(cap);

        /* set first two terms */
        cache[0] = (seq_t)2;          /* a(0) */
        cache[1] = (seq_t)7;          /* a(1) */

        last_index   = 1;
        initialized  = true;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        grow_seq(&cache, &cap, (size_t)n + 1);
    }

    /* compute missing terms up to n */
    if (last_index < n) {
        for (idx_t i = last_index + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)10 - (seq_t)13;
        }
        last_index = n;
    }

    return cache[n];
}
