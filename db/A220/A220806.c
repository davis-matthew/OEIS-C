/*
 * A220806  Equals one maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, vertical and antidiagonal neighbors in a random 0..2 n
 *          X 2 array.
 * https://oeis.org/A220806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(1) = 2, a(2) = 8, a(3) = 38, a(4) = 168
 *            a(n) = 7*a(n-1) - 11*a(n-2) - 3*a(n-3) - 4*a(n-4) for n >= 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;      /* 0‑based storage: cache[0] = a(1) */
    static idx_t cap = 0;            /* number of allocated elements */

    /* initialise cache with the first 4 known values */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* minimal error handling */
        cache[0] = (seq_t)2;      /* a(1) */
        cache[1] = (seq_t)8;      /* a(2) */
        cache[2] = (seq_t)38;     /* a(3) */
        cache[3] = (seq_t)168;    /* a(4) */
    }

    /* Return cached value if already computed */
    if (n <= cap) {
        return cache[n - 1];
    }

    /* Extend cache up to n */
    idx_t old_cap = cap;
    cap = n;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) return 0;     /* minimal error handling */
    cache = new_cache;

    /* Compute values from old_cap+1 to n using the recurrence */
    for (idx_t i = old_cap + 1; i <= n; ++i) {
        idx_t idx = i - 1; /* 0‑based index for cache */
        seq_t val = 0;
        /* a(i) = 7*a(i-1) - 11*a(i-2) - 3*a(i-3) - 4*a(i-4) */
        val = 7 * cache[idx - 1]
            - 11 * cache[idx - 2]
            - 3  * cache[idx - 3]
            - 4  * cache[idx - 4];
        cache[idx] = val;
    }

    return cache[n - 1];
}
