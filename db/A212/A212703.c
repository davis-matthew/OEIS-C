/*
 * A212703  Main transitions in systems of n particles with spin 4.
 * https://oeis.org/A212703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence A212703 (Main transitions in systems of n particles with spin 4)
   Formula: a(n) = n * (9-1) * 9^(n-1) = 8 * n * 9^(n-1)
   Recurrence: a(1)=8, a(2)=144, a(n)=18*a(n-1)-81*a(n-2)  (n>2) */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n = 0, return 0 */

    /* static cache storage for memoization */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;          /* allocated capacity */
    static size_t count   = 0;          /* number of terms currently stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap   = 2;                       /* start with space for 2 terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        count = 0;
    }

    /* grow cache if needed */
    if (cap < (size_t)n) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < count; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise the first two terms if not already present */
    if (count < 1) { cache[0] = (seq_t)8;  count = 1; }       /* a(1) */
    if (count < 2) { cache[1] = (seq_t)144; count = 2; }     /* a(2) */

    /* compute any missing terms up to n */
    for (size_t i = count; i < (size_t)n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)18 * prev - (seq_t)81 * prev2;
    }
    count = (size_t)n;

    return cache[n - 1];
}
