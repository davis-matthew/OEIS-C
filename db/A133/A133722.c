/*
 * A133722  Column 3 of triangle in A133721.
 * https://oeis.org/A133722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-3) - 10*a(n-6) + 10*a(n-9) - 5*a(n-12) + a(n-15)
 *            for n > 15, with initial values a(1) = 0, a(2) = 0, a(3) = 1,
 *            a(4) = 1, a(5) = 1, a(6) = 1, a(7) = 7, a(8) = 3, a(9) = 1,
 *            a(10) = 25, a(11) = 6, a(12) = 1, a(13) = 65, a(14) = 10, a(15)
 *            = 1. Indexing is 1‑based
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding already computed values (1‑based). */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;  /* highest n for which cache[n] is valid */

    if (n == 0) return 0;  /* sequence is defined for n ≥ 1 */

    if (n > max_cached) {
        /* Ensure the cache can hold indices up to n (size n+1). */
        size_t new_size = (size_t)(n + 1);           /* +1 for index n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        /* Zero‑initialize the newly allocated entries. */
        for (size_t i = (size_t)max_cached + 1; i <= (size_t)n; ++i)
            cache[i] = 0;

        /* Initial values for 1 … 15. */
        const seq_t init[16] = { 0, 0, 0, 1, 1, 1, 1, 7, 3, 1,
                                 25, 6, 1, 65, 10, 1 };
        for (idx_t i = 1; i <= 15 && i <= n; ++i)
            cache[(size_t)i] = init[(size_t)i];

        /* Compute the recurrence for all indices > 15 up to n. */
        for (idx_t i = 16; i <= n; ++i) {
            seq_t val = 5 * cache[(size_t)(i - 3)];
            val += 10 * cache[(size_t)(i - 9)];
            val += cache[(size_t)(i - 15)];
            val -= 10 * cache[(size_t)(i - 6)];
            val -= 5  * cache[(size_t)(i - 12)];
            cache[(size_t)i] = val;
        }

        max_cached = n;
    }

    return cache[(size_t)n];
}
