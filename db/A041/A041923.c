/*
 * A041923  Denominators of continued fraction convergents to sqrt(483).
 * https://oeis.org/A041923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041923
   Denominators of continued fraction convergents to sqrt(483)
   Recurrence: a(n) = 44*a(n-2) - a(n-4)  for n > 3
   Initial values: a(0)=1, a(1)=1, a(2)=43, a(3)=44
   The implementation uses a growing cache so subsequent calls are
   O(1) after the first request for a given n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated array size */
    static idx_t  len = 0;       /* highest index stored + 1 */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)43;
        cache[3] = (seq_t)44;
        len = 4;
    }

    /* grow the cache if n is outside the current capacity */
    if ((size_t)n >= capacity) {
        size_t need = (size_t)n + 1;
        size_t new_cap = capacity * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = (seq_t)44 * cache[i - 2] - cache[i - 4];
    }

    /* keep track of the maximum index computed so far */
    if ((idx_t)n + 1 > len) len = (idx_t)n + 1;

    return cache[n];
}
