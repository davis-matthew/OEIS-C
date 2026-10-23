/*
 * A106329  Numbers k such that k^2 = 8*j^2 + 9.
 * https://oeis.org/A106329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3, a(2) = 9, a(n) = 6*a(n-1) - a(n-2) for n > 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static memoization to make calls independent of order */
    static seq_t *memo = NULL;
    static idx_t capacity = 0;            /* Allocated size of the array (indices 0..capacity-1) */
    static idx_t last_comp = 0;           /* Highest index computed and stored in memo */

    /* Ensure initialization */
    if (!memo) {
        capacity = 4;                     /* initial capacity (indices 0..3) */
        memo = (seq_t *)malloc(sizeof(seq_t) * capacity);
        memo[1] = 3;                      /* a(1) */
        memo[2] = 9;                      /* a(2) */
        last_comp = 2;
    }

    /* Expand allocation if needed */
    if (n >= capacity) {
        while (n >= capacity) capacity <<= 1;   /* double until large enough */
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * capacity);
    }

    /* Compute missing terms up to n */
    if (n > last_comp) {
        for (idx_t i = last_comp + 1; i <= n; ++i) {
            memo[i] = 6 * memo[i - 1] - memo[i - 2];
        }
        last_comp = n;
    }

    return memo[n];
}
