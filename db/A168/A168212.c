/*
 * A168212  a(n) = 7*n - a(n-1) + 1 with n>1, a(1)=4.
 * https://oeis.org/A168212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n - a(n-1) + 1; a(1) = 4 (offset 1)
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cached = 0;       /* highest index computed so far   */
    static size_t cap = 0;         /* allocated size of the cache      */

    if (n == 0)   /* the sequence is undefined for n < 1, but return 0 anyway */
        return (seq_t)0;

    /* Grow the cache if needed */
    if ((idx_t)cap <= n) {
        size_t new_cap = n + 1;          /* indices 0 .. n inclusive   */
        if (cache == NULL) {
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (cache == NULL) /* allocation failure */
            return (seq_t)0;
        /* ensure the low zero slot is defined (even though unused) */
        cache[0] = (seq_t)0;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)4;
        else
            cache[i] = 7 * (seq_t)i - cache[i - 1] + 1;
    }
    cached = n;

    return cache[n];
}
