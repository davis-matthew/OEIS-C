/*
 * A029053  Expansion of 1/((1-x)(1-x^3)(1-x^7)(1-x^12)).
 * https://oeis.org/A029053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts partitions of n into parts 1, 3, 7 and 12
   – i.e. the coefficient of xⁿ in 1/((1‑x)(1‑x³)(1‑x⁷)(1‑x¹²)). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed DP values */
    static seq_t *dp = NULL;
    static idx_t cap = 0;          /* maximum n for which dp is valid */

    /* ensure enough space for 0 .. n */
    if (dp == NULL || n > cap) {
        /* allocate or enlarge array */
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure – return 0 */
        dp  = tmp;
        cap = n;
    }

    /* initialise all entries to 0 and set dp[0] = 1 */
    dp[0] = 1;
    for (idx_t i = 1; i <= cap; ++i) dp[i] = 0;

    /* parts allowed in the partitions */
    static const idx_t parts[4] = {1, 3, 7, 12};

    /* classic 1‑D coin–change DP */
    for (int k = 0; k < 4; ++k) {
        idx_t part = parts[k];
        for (idx_t i = part; i <= cap; ++i)
            dp[i] += dp[i - part];
    }

    return dp[n];
}
