/*
 * A163864  a(n) = 2*a(n-2) for n > 2; a(1) = 1, a(2) = 6.
 * https://oeis.org/A163864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) for n > 2; a(1) = 1, a(2) = 6, n >= 1
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t computed = 0;       /* highest index computed so far   */
    static size_t cap = 0;           /* size of allocated cache in slots */

    if (n == 0)                       /* undefined; return 0 */
        return (seq_t)0;

    /* Ensure the cache can hold indices up to n (plus 1 for safety). */
    size_t needed = (size_t)n + 2;     /* indices 0..n inclusive */
    if (cache == NULL || cap < needed) {
        size_t new_cap = (cap > needed) ? cap : needed;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)              /* allocation failure */
            return (seq_t)0;
        cache = (seq_t *)tmp;
        cap = new_cap;
    }

    /* Initialise the first two values if not already done. */
    if (computed < 1) {
        cache[1] = (seq_t)1;
        computed = 1;
    }
    if (computed < 2) {
        cache[2] = (seq_t)6;
        computed = 2;
    }

    /* Fill in any missing terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 2];  /* 2 * cache[i-2] */
    }
    computed = (n > computed) ? n : computed;

    return cache[n];
}
