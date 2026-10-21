/*
 * A099463  Bisection of tribonacci numbers.
 * https://oeis.org/A099463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + a(n-3) with a(0) = 0, a(1) = 1, a(2)
 *            = 2 (for all n >= 3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Memoized iterative implementation. */
seq_t a(idx_t n)
{
    /* Static cache: holds computed a(i) for i < used.
       The array is grown on demand. */
    static seq_t *cache = NULL;
    static size_t used = 0;   /* number of values stored */
    static size_t cap = 0;    /* allocated capacity */

    /* Ensure initial values are present. */
    if (used == 0) {
        cap = 8;               /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        used = 3;
    }

    /* Grow cache if necessary. */
    while ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        /* In case of overflow, just double; cap is size_t so fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (size_t i = used; i <= (size_t)n; ++i) {
        /* Handle small indices directly. */
        if (i == 0) { cache[0] = 0; continue; }
        if (i == 1) { cache[1] = 1; continue; }
        if (i == 2) { cache[2] = 2; continue; }

        /* Recurrence: a(i) = 3*a(i-1) + a(i-2) + a(i-3) */
        seq_t val = 3 * cache[i - 1] + cache[i - 2] + cache[i - 3];
        cache[i] = val;
    }
    used = (size_t)n + 1;   /* all values up to n are now valid */

    return cache[(size_t)n];
}
