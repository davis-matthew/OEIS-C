/*
 * A025856  Expansion of 1/((1-x^3)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A025856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A025856
 *  a(n) = number of partitions of n into parts 3, 10, and 11
 *  (i.e. the number of ways to write n as a sum of 3, 10 and 11, where
 *  the order of summands does not matter).
 *  This is a standard coin–change counting problem. */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;     /* dynamically sized table of partition counts */
static idx_t dp_size = 0;    /* largest n for which dp[n] is valid */

/*  Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* If this is the first call or we need to cover a larger n, recompute. */
    if (dp == NULL || n > dp_size) {
        /* Allocate or enlarge the table. */
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (tmp == NULL)      /* allocation failed – return 0 as error value */
            return 0;
        dp = tmp;

        /* Initialise all entries to 0, then set the base case. */
        memset(dp, 0, sizeof(seq_t) * (n + 1));
        dp[0] = 1;          /* one way to make 0: use no parts */

        /* Count combinations using the coin–change method. */
        {
            /* Treat 3 as the first coin */
            for (idx_t j = 3; j <= n; ++j)
                dp[j] += dp[j - 3];
            /* Then 10 */
            for (idx_t j = 10; j <= n; ++j)
                dp[j] += dp[j - 10];
            /* Finally 11 */
            for (idx_t j = 11; j <= n; ++j)
                dp[j] += dp[j - 11];
        }

        dp_size = n;        /* update the cached maximum */
    }

    /* For n <= dp_size we already have the correct value. */
    return dp[n];
}
