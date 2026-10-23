/*
 * A178853  "Josephus problem": n persons stand in a circle and eliminate
 *          every seventh person counting clockwise until only person a(n) is
 *          remaining.
 * https://oeis.org/A178853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
   a(n) = ((a(n-1) - 1 + 7) % n) + 1   for n ≥ 2
   Josephus problem with step 7, survivor numbering starting at 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static size_t size  = 0;   /* number of computed values */
    static size_t cap   = 0;   /* allocated capacity */

    if (n == 0) return 0;      /* undefined, guard against underflow */

    /* Ensure enough space for indices up to n */
    if (cap < n) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the first value if nothing computed yet */
    if (size == 0) {
        cache[0] = 1;
        size = 1;
    }

    /* Compute missing values up to n */
    for (idx_t k = size + 1; k <= n; ++k) {
        seq_t prev = cache[size - 1]; /* a(k-1) */
        seq_t val  = ((prev - 1 + 7) % (seq_t)k) + 1;
        cache[size] = val;
        ++size;
    }

    return cache[n - 1];
}
