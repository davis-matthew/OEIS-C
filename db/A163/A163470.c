/*
 * A163470  a(n) = 8*a(n-1) - 13*a(n-2) for n > 1; a(0) = 3, a(1) = 15.
 * https://oeis.org/A163470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 13*a(n-2); a(0) = 3, a(1) = 15 */
#include <stdlib.h> /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache for already computed values */
    static seq_t *cache = NULL;          /* array of a(k) */
    static size_t cap = 0;               /* allocated size of cache */
    static idx_t max_calculated = 1;     /* largest n whose value is in cache */

    /* Base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)15;

    /* Lazily allocate cache for the first two terms */
    if (!cache) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)15;
    }

    /* Return cached value if it already exists */
    if ((size_t)n < cap)
        return cache[n];

    /* Grow cache to accommodate index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap   = new_cap;

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_calculated + 1; i <= n; ++i)
        cache[i] = 8 * cache[i - 1] - 13 * cache[i - 2];

    max_calculated = n;
    return cache[n];
}
