/*
 * A029307  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by partition DP: a(n) = number of partitions of n
   into parts 3, 7, 8 and 10.  The function recomputes the entire
   table up to the maximum n requested and memoizes it between calls. */

#include <stdlib.h>  /* realloc, abort */

seq_t a(idx_t n) {
    /* static state */
    static seq_t *dp = NULL;
    static size_t capacity = 0;    /* array capacity */
    static idx_t max_n = 0;        /* largest n already computed */
    const idx_t coins[4] = {3, 7, 8, 10};

    /* enlarge the array if necessary */
    if (capacity < (size_t)(n + 1)) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        /* zero the newly allocated part */
        for (size_t i = capacity; i < new_cap; ++i) tmp[i] = 0;
        dp = tmp;
        capacity = new_cap;
    }

    /* initialise dp[0] once */
    if (dp[0] == (seq_t)0) dp[0] = 1;

    /* if a larger n is requested, recompute the table up to n */
    if (n > max_n) {
        /* zero entries that will be recomputed */
        for (idx_t i = 1; i <= n; ++i) dp[i] = 0;
        /* classic coin–change DP: iterate coins outer, amounts inner */
        for (int k = 0; k < 4; ++k) {
            idx_t c = coins[k];
            for (idx_t i = c; i <= n; ++i) {
                dp[i] += dp[i - c];
            }
        }
        max_n = n;   /* update the stored maximum */
    }

    return dp[n];
}
