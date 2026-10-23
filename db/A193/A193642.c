/*
 * A193642  Number of arrays of -2..2 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3), a(1) = 1, a(2) = 5, a(3) =
 *            13
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoisation array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated slots */
    static idx_t cache_len = 0;    /* number of terms stored */

    if (n == 0) /* Not used, guard against accidental zero input */
        return 0;

    /* Grow cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)1;             /* a(1) */
        else if (i == 2)
            val = (seq_t)5;             /* a(2) */
        else if (i == 3)
            val = (seq_t)13;            /* a(3) */
        else {
            /* recurrence: a(i) = 2*a(i-1) + 2*a(i-2) + a(i-3) */
            val = 2 * cache[i - 2]         /* a(i-1) */
                + 2 * cache[i - 3]         /* a(i-2) */
                +  cache[i - 4];           /* a(i-3) */
        }
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
