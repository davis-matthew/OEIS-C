/*
 * A178094  a(1)=a(2)=1; thereafter a(n) = lpf(a(n-1)+a(n-2)), where lpf =
 *          "least prime factor".
 * https://oeis.org/A178094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(n)=lpf(a(n-1)+a(n-2)) for n≥3 */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    /* static cache: index 0 unused; cache[1] and cache[2] are the base cases   */
    static seq_t *cache = NULL;   /* pointer to an array of seq_t */
    static size_t  cap   = 0;     /* capacity of the array (number of elements) */

    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Ensure the cache can hold index n.  Index 0 is unused. */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;                  /* index 0 unused       */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                            /* allocation failure   */
        /* initialise newly allocated cells to 0 (uncomputed) */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first two terms if not already set */
    if (cap > 1 && cache[1] == 0)
        cache[1] = (seq_t)1;
    if (cap > 2 && cache[2] == 0)
        cache[2] = (seq_t)1;

    /* compute missing terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] == 0) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            cache[i]  = (seq_t)lpf((idx_t)sum);
        }
    }

    return cache[n];
}
