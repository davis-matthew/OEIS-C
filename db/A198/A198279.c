/*
 * A198279  Number of 2n X 4 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 8*a(n-3) Initial values: a(1)=1,
 *            a(2)=4, a(3)=14
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache for memoisation (1‑based indices) */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) for 1 <= i <= cap */
    static idx_t cap   = 0;        /* maximum allocated index (inclusive)  */
    static idx_t last  = 0;        /* largest index that has been computed  */

    /* Determine how many indices we must store (at least n, and 3 for init) */
    idx_t need = n;
    if (need < 3) need = 3;

    /* Grow the cache if necessary */
    if (cap < need) {
        idx_t newcap = cap ? cap : 1;
        while (newcap < need) newcap <<= 1;          /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise starting values if not yet done */
    if (last < 3) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        last    = 3;
    }

    /* If the requested value is already computed, just return it */
    if (n <= last) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v3 = cache[i - 3];
        seq_t v2 = cache[i - 2];
        seq_t v1 = cache[i - 1];
        cache[i] = 4 * v1 + v2 - 8 * v3;
    }
    last = n;
    return cache[n];
}
