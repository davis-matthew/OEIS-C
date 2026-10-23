/*
 * A025874  Expansion of 1/((1-x^4)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A025874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-4) + a(n-9) + a(n-12) - a(n-13) - a(n-16) - a(n-21) + a(n-25)
 *
 * Initial value: a(0) = 1
 * For n < 0 the terms are taken as 0.
 *
 * This implementation uses memoisation with a static dynamic array.
 * It works for any build where seq_t is an unsigned integer type (8, 16,
 * 32, 64 or 128 bits) or an arbitrary-precision integer class.
 */

#include <stdlib.h>   /* for realloc, size_t */

seq_t a(idx_t n)
{
    /* static cache storing already computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of elements currently allocated */

    /* If we already have the answer, just return it */
    if (n < (idx_t)cap)
        return cache[(size_t)n];

    /* grow the cache to hold indices up to n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        return 0;   /* out-of-memory: return 0 (should not happen for the tests) */
    cache = tmp;

    /* Compute new entries iteratively, using the recurrence.
     * All references are guaranteed to be non‑negative because we
     * only compute for indices >= 0 and skip negative sub‑indices. */
    for (size_t i = cap; i < new_cap; ++i)
    {
        seq_t val = 0;

        if (i >= 4)
            val += cache[i - 4];
        if (i >= 9)
            val += cache[i - 9];
        if (i >= 12)
            val += cache[i - 12];
        if (i >= 13)
            val -= cache[i - 13];
        if (i >= 16)
            val -= cache[i - 16];
        if (i >= 21)
            val -= cache[i - 21];
        if (i >= 25)
            val += cache[i - 25];

        /* Initialise the very first term */
        if (i == 0)
            val = 1;

        cache[i] = val;
    }

    cap = new_cap;
    return cache[(size_t)n];
}
