/*
 * A022378  Fibonacci sequence beginning 2, 32.
 * https://oeis.org/A022378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 32
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle base cases immediately */
    if (n < 2)
        return (n == 0) ? (seq_t)2 : (seq_t)32;

    /* Static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of entries currently filled   */
    static size_t cap = 0;   /* allocated capacity of the array       */

    /* Ensure the array has space for indices up to n */
    if (cap < (size_t)n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) {             /* allocation failed – aborting */
            abort();
        }
        cache = tmp;
        cap = (size_t)n + 1;
    }

    /* Initialise the first two terms if not already done */
    if (len < 2) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)32;
        len = 2;
    }

    /* Compute missing terms up to n using the Fibonacci recurrence */
    for (idx_t i = (idx_t)len; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2];

    len = (size_t)n + 1;   /* we now have values for 0..n */

    return cache[n];
}
