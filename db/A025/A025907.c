/*
 * A025907  Expansion of 1/((1-x^6)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 6, 11, and 12 (OEIS
 *            A025907)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  dp[i] holds the number of partitions of i using parts {6,11,12}.  */
static seq_t *dp = NULL;   /* dynamic table allocated on first call or when more space is needed */
static idx_t  dp_size = 0; /* number of valid entries in dp (dp_size = max_n + 1) */

seq_t a(idx_t n)
{
    /* Allocate or enlarge the DP table as necessary.  */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;

        /* Zero the table up to new_size.  */
        for (idx_t i = 0; i < new_size; ++i)
            dp[i] = 0;
        dp[0] = 1; /* empty partition */

        /* Fill the table using the classic coin‑change DP (order
           of parts does not matter).  */
        const idx_t parts[3] = {6, 11, 12};
        for (int p = 0; p < 3; ++p) {
            idx_t coin = parts[p];
            for (idx_t i = coin; i <= n; ++i)
                dp[i] += dp[i - coin];
        }

        dp_size = new_size;
    }

    return dp[n];
}
