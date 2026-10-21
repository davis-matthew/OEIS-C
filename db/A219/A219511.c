/*
 * A219511  Pierce expansion of 144 - 64*sqrt(5).
 * https://oeis.org/A219511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9; for n>=2: if n even: a(n)=4*(a(n-1)+1); if n
 *            odd: a(n)=2*(a(n-2))^2-1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t capacity = 0;     /* allocated capacity (elements) */
    static size_t used = 0;         /* number of elements actually computed */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        capacity = 8;                 /* arbitrary start size */
        cache = alloc_seq_array(capacity);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        used = 2;
    }

    /* enlarge cache if necessary to hold index n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;          /* double until enough */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) abort();
        /* copy existing values */
        for (size_t i = 0; i < used; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* compute missing terms */
    for (size_t i = used; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)9;
        } else if ((i & 1) == 0) {            /* even index */
            cache[i] = 4 * (cache[i - 1] + 1);
        } else {                              /* odd index */
            seq_t t = cache[i - 2];
            cache[i] = 2 * t * t - 1;
        }
    }
    used = (size_t)n + 1;
    return cache[(size_t)n];
}
