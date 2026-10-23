/*
 * A042403  Denominators of continued fraction convergents to sqrt(728).
 * https://oeis.org/A042403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042403 – Denominators of continued fraction convergents to sqrt(728)
 * Recurrence: a(0)=1, a(1)=1, a(2)=53, a(3)=54, a(n)=54*a(n-2)-a(n-4) for n>3
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size   */
    static size_t next_to_compute = 0;    /* smallest index not yet computed */

    size_t target = (size_t)n;            /* avoid signed/unsigned mix */

    /* Allocate or enlarge the cache if needed */
    if (target + 1 > cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed – return 0 as error */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to the requested index */
    for (size_t i = next_to_compute; i <= target; ++i) {
        if (i == 0)          cache[0] = (seq_t)1;
        else if (i == 1)     cache[1] = (seq_t)1;
        else if (i == 2)     cache[2] = (seq_t)53;
        else if (i == 3)     cache[3] = (seq_t)54;
        else                 cache[i] = ((seq_t)54 * cache[i-2]) - cache[i-4];
    }
    next_to_compute = target + 1;

    return cache[target];
}
