/*
 * A023112  Squares that remain square when the digit 4 is appended.
 * https://oeis.org/A023112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = y_n²
 *  where y_1 = 0, y_2 = 12 and for n ≥ 3
 *        y_n = 38·y_{n-1} – y_{n-2}
 *  Thus a(n) gives the squares that remain square when a 4 is appended. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Calls with n < 1 return 0. */
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)0;            /* y_1 = 0  →  a(1) = 0    */
    if (n == 2) return (seq_t)144;          /* y_2 = 12 →  a(2) = 144 */

    /* Static cache of y-values, 1‑based indexing. */
    static seq_t *y_cache = NULL;  /* array of y-values */
    static idx_t cap = 0;          /* allocated capacity (≥ n+1) */
    static idx_t sz  = 0;          /* highest index computed */

    /* Ensure enough storage.  Grow by doubling for amortised O(1). */
    if (cap < n + 1) {
        idx_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;          /* new_cap > n */
        y_cache = (seq_t*)realloc(y_cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise the base cases the first time they are needed. */
    if (sz < 1) y_cache[1] = (seq_t)0;
    if (sz < 2) y_cache[2] = (seq_t)12;

    /* Compute any missing y-values up to n. */
    if (sz < n) {
        idx_t start = (sz < 3) ? 3 : sz + 1;
        for (idx_t i = start; i <= n; ++i) {
            y_cache[i] = (seq_t)38 * y_cache[i - 1] - y_cache[i - 2];
        }
        sz = n;
    }

    seq_t y = y_cache[n];
    return y * y;
}
