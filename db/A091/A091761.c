/*
 * A091761  a(n) = Pell(4n) / Pell(4).
 * https://oeis.org/A091761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=34*a(n-1)-a(n-2) */
#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* total allocated capacity */
    static size_t computed = 0; /* number of valid entries */

    /* Initialise base cases if this is the first call */
    if (cap == 0) {
        /* Allocate space for two elements */
        ensure_capacity(2, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* Return cached value if we already computed it */
    if ((size_t)n < computed)
        return cache[n];

    /* Grow the array to hold the requested index */
    if ((size_t)n + 1 > cap) {
        ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* Compute new entries using the recurrence */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)34 * cache[i - 1] - cache[i - 2];
    }

    computed = (size_t)n + 1;
    return cache[n];
}
