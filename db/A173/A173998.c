/*
 * A173998  For n>=1, a(n) = n + 2 + Sum_{i=1..n-1} a(i)*a(n-i).
 * https://oeis.org/A173998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive formula:
 *   a(1) = 3
 *   a(n) = n + 2 + Σ_{i=1}^{n-1} a(i) * a(n-i)   for n ≥ 1
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[0] is unused; cache[1] .. cache[n] hold a(n) */
static size_t  cap   = 0;        /* number of allocated slots (indices 0 .. cap-1) */
static idx_t   last_computed = 0; /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* if n is already computed, return immediately */
    if (n <= last_computed)
        return cache[n];

    /* Ensure the cache can hold indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;
        if (new_cap < old_cap * 2) new_cap = old_cap * 2;
        if (new_cap == 0) new_cap = 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* allocation failure – free and return 0 */
            free(cache);
            cache = NULL;
            cap   = 0;
            last_computed = 0;
            return (seq_t)0;
        }

        /* initialise any newly allocated cells to 0 */
        for (size_t i = old_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;

        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all new terms up to n */
    idx_t start = (last_computed < 1) ? 1 : last_computed + 1;
    for (idx_t k = start; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i)
            sum += cache[i] * cache[k - i];
        cache[k] = (seq_t)k + (seq_t)2 + sum;
    }
    last_computed = n;
    return cache[n];
}
