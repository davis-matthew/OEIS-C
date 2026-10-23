/*
 * A173808  a(n) = (7*10^n + 11)/9 for n > 0.
 * https://oeis.org/A173808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*10^n + 11)/9, n > 0 (A173808) See also recurrence:
 *            a(0) = 2, a(n) = 10*a(n-1) - 11 for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 2; /* defined only for the recurrence base */

    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t  max_n = 0;      /* largest n for which cache is filled */
    static size_t cap   = 0;      /* current cache capacity */

    /* Resize cache if necessary */
    if (n > cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – unlikely in test harness */
        cache = tmp;
        cap   = new_cap;
    }

    /* If n has already been computed, return it immediately */
    if (n <= max_n) return cache[n-1];

    /* Start from the largest previously computed value */
    seq_t val;
    if (max_n == 0) {          /* no value yet – initialise a(1) */
        cache[0] = 9;          /* a(1) = 9 */
        val     = 9;
        max_n   = 1;
    } else {
        val = cache[max_n-1];
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        val = val * 10 - 11;   /* a(i) = 10*a(i-1) - 11 */
        cache[i-1] = val;
    }
    max_n = n;

    return cache[n-1];
}
