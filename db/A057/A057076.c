/*
 * A057076  A Chebyshev or generalized Fibonacci sequence.
 * https://oeis.org/A057076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 11, a(n) = 11*a(n-1) - a(n-2) for n ≥ 2 (linear
 *            recurrence from the G.f.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t next = 0;      /* next index yet to be computed in cache */
    static size_t cap = 0;      /* allocated capacity of cache */

    /* Ensure cache size is enough for index n */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 2;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialize first two terms on first use */
    if (next == 0) {
        cache[0] = 2;
        cache[1] = 11;
        next = 2;
    }

    /* Compute terms up to n if not already available */
    if (n >= next) {
        for (idx_t i = next; i <= n; ++i) {
            /* recurrence: a(i) = 11*a(i-1) - a(i-2) */
            cache[i] = 11 * cache[i - 1] - cache[i - 2];
        }
        next = n + 1;
    }

    return cache[n];
}
