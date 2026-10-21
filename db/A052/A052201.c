/*
 * A052201  Equivalent of the Kurepa hypothesis for left factorial.
 * https://oeis.org/A052201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * ( a(n-1) - a(n-3) ) - 3 * a(n-2) Initial values:
 *            a(1)=4, a(2)=15, a(3)=48
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized iterative computation of A052201 */
seq_t a(idx_t n)
{
    /* base case: indices < 1 are undefined, return 0 */
    if (n < 1) return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;           /* allocated size (index 0 unused) */

    /* ensure space for at least n+1 elements (index 0 unused) */
    if (cap < n + 1) {
        size_t new_cap = cap == 0 ? 4 : cap;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first three values once */
    if (cache[1] == 0) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)48;
    }

    /* compute missing values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = cache[i - 1] - cache[i - 3];
        seq_t term2 = (seq_t)(i + 1) * term1;
        seq_t term3 = (seq_t)3 * cache[i - 2];
        cache[i] = term2 - term3;
    }

    return cache[n];
}
