/*
 * A159022  a(0)=29; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=29; a(n)=a(n‑1)+floor(sqrt(a(n‑1))), n>0
 * Range:     u8 n <= 6, u16 n <= 183, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;      /* dynamic array of computed terms   */
static idx_t  cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity in elements     */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        /* Allocate at least one extra to avoid repeated tiny reallocs */
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first element if this is the first call */
    if (cache_len == 0) {
        cache[0] = 29;            /* a(0) */
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    while (cache_len <= n) {
        seq_t prev = cache[cache_len - 1];
        seq_t inc  = floor_sqrt(prev);        /* floor(sqrt(prev)) */
        cache[cache_len] = prev + inc;
        cache_len++;
    }

    return cache[n];
}
