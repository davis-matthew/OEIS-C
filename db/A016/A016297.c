/*
 * A016297  Expansion of 1/((1-2*x) * (1-5*x) * (1-8*x)).
 * https://oeis.org/A016297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 66*a(n-2) + 80*a(n-3) a(0) = 1, a(1) = 15,
 *            a(2) = 159, n ≥ 3 Use a memoised recurrence to answer queries in
 *            any order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  sz     = 0;

    /* Ensure capacity for at least n+1 entries. */
    if (cap < n + 1) {
        size_t newcap = cap ? cap * 2 : 64;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(*cache));
        cap = newcap;
    }

    /* Initialise base cases if not already done. */
    if (sz == 0) {                          /* a(0) */
        cache[0] = (seq_t)1;
        sz = 1;
    }
    if (sz == 1) {                          /* a(1) */
        cache[1] = (seq_t)15;
        sz = 2;
    }
    if (sz == 2) {                          /* a(2) */
        cache[2] = (seq_t)159;
        sz = 3;
    }

    /* Compute missing values up to n. */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        /* 15*a1 + 80*a3 - 66*a2 */
        cache[i] = (seq_t)15 * a1 + (seq_t)80 * a3 - (seq_t)66 * a2;
    }
    sz = n + 1;
    return cache[n];
}
