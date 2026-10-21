/*
 * A075091  Sum of Lucas numbers and reflected Lucas numbers (comment to
 *          A061084).
 * https://oeis.org/A075091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((-1)^n + 1) * L(n), where L(n) is the n‑th Lucas number.
 *            Equivalent recurrence for even indices: a(0)=4, a(2)=6,
 *            a(n)=3*a(n-2)-a(n-4) for n≥4 (odd n produce 0)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 45, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Dynamic cache of previously computed values. */
    static seq_t *cache = NULL;    /* array of seq_t values */
    static size_t  capacity = 0;   /* allocated size of cache  */
    static idx_t   max_computed = -1;  /* last index that has been filled */
    static bool    initialized = false;

    /* Ensure the cache is large enough for indices up to n. */
    size_t new_cap = n + 1;
    if (new_cap < 4)               /* we store the first four terms at least */
        new_cap = 4;

    if (new_cap > capacity) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise the base terms the first time the function is called. */
    if (!initialized) {
        cache[0] = (seq_t)4;   /* 2·L(0) */
        cache[1] = (seq_t)0;   /* odd indices → 0 */
        cache[2] = (seq_t)6;   /* 2·L(2) */
        cache[3] = (seq_t)0;   /* odd indices → 0 */
        max_computed = 3;
        initialized = true;
    }

    /* If the requested term is already in the cache, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if ((i & 1) == 1) {                   /* odd indices → 0 */
            cache[i] = (seq_t)0;
        } else {                              /* even indices use recurrence */
            cache[i] = (seq_t)3 * cache[i - 2] - cache[i - 4];
        }
    }
    max_computed = n;

    return cache[n];
}
