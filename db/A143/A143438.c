/*
 * A143438  a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6), with a(0) = a(2) = a(3)
 *          = 1, a(1) = 0 and a(4) = a(5) = 2.
 * https://oeis.org/A143438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6); initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=2. The function is
 *            memoised and can be called in any order
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 68, u64 n <= 134,
 *            u128 n <= 265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (number of elems)   */
    static idx_t ready  = 0;   /* next index that must be computed      */

    /* Expand cache if it cannot hold n+1 elements. */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = ready; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = 1;
        else if (i == 1)     val = 0;
        else if (i == 2)     val = 1;
        else if (i == 3)     val = 1;
        else if (i == 4)     val = 2;
        else if (i == 5)     val = 2;
        else                 val = cache[i-2] + cache[i-3] + cache[i-4] - cache[i-6];
        cache[i] = val;
    }
    ready = (n + 1) > ready ? (n + 1) : ready;   /* ensure ready holds max computed */

    return cache[n];
}
