/*
 * A036232  a(n+1) = a(n) + sum of digits of a(n) starting with 211.
 * https://oeis.org/A036232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=211, a(n+1)=a(n)+digit_sum(a(n)) */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache: cache[i] == a(i) for i >= 1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t size = 0;  /* one past the last initialized element */

    if (n == 0) return 0;   /* Undefined for n < 1, return 0 as a neutral value */

    /* First call: initialize cache with the first term */
    if (cache == NULL) {
        cap = 32;                  /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;      /* Allocation failure */
        cache[1] = 211;            /* a(1) = 211 */
        size = 2;                  /* next free index is 2 */
    }

    /* Grow capacity if necessary */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= (idx_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* Reallocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        cache[(size_t)i] = prev + digit_sum(prev);
    }
    size = n + 1; /* update next free index */

    return cache[(size_t)n];
}
