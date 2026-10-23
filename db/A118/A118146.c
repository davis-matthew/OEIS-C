/*
 * A118146  Start with 1 and repeatedly reverse the digits and add 49 to get
 *          the next term.
 * https://oeis.org/A118146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = reverse_decimal(a(n-1)) + 49. */
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Cache for memoisation: cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static idx_t last = 0;    /* last computed index (1‑based) */
    static size_t cap = 0;    /* allocated capacity */

    if (n == 0) {
        return (seq_t)0;   /* behavior below the offset is irrelevant */
    }

    /* Initialise cache on first call. */
    if (last == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(1) */
        last = 1;
    }

    /* Extend capacity if necessary. */
    if (n > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n - 1];   /* keep existing cache on failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    while (last < n) {
        seq_t prev = cache[last - 1];            /* a(last)          */
        seq_t rev  = reverse_decimal(prev);     /* rev(a(last))     */
        cache[last] = rev + (seq_t)49;           /* a(last+1)        */
        ++last;
    }

    return cache[n - 1];
}
