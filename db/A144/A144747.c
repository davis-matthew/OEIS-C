/*
 * A144747  Recurrence sequence a(n)=a(n-1)^2-a(n-1)-1, a(0)=7.
 * https://oeis.org/A144747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-1) - 1, a(0) = 7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;      /* array of a(0) .. a(k) */
    static size_t cap   = 0;         /* allocated size of cache   */
    static size_t next  = 0;         /* index of next value to fill (next unused index) */

    /* Allocate or grow cache if necessary */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)            /* allocation failed */
            return (seq_t)0; /* very unlikely in the test environment */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = next; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)7;
        } else {
            seq_t prev = cache[i - 1];
            cache[i] = prev * prev - prev - (seq_t)1;
        }
    }
    next = (size_t)n + 1;     /* all values up to n are now computed */

    return cache[(size_t)n];
}
