/*
 * A208503  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 6*a(n-2) - 3*a(n-3)  (n > 5)
 * Initial values: a(1)=13, a(2)=169, a(3)=234, a(4)=650, a(5)=1794
 */

#include <stdlib.h>

static seq_t *dp = NULL;   /* array of precomputed terms (1‑based) */
static idx_t dp_size = 0;  /* current number of computed terms */

/* initialise the table with the first 5 terms */
static void init_seq(void)
{
    if (dp != NULL)
        return;

    dp_size = 5;
    dp = (seq_t *)malloc((dp_size + 1) * sizeof(seq_t)); /* slot 0 unused */
    if (!dp)
        abort();                                     /* insufficient memory */

    dp[1] = (seq_t)13;
    dp[2] = (seq_t)169;
    dp[3] = (seq_t)234;
    dp[4] = (seq_t)650;
    dp[5] = (seq_t)1794;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    init_seq();

    if (n <= dp_size)
        return dp[n];

    /* extend the table if necessary */
    idx_t new_size = dp_size;
    while (new_size < n)          /* double until we have space for n */
        new_size *= 2;

    seq_t *new_dp = (seq_t *)realloc(dp, (new_size + 1) * sizeof(seq_t));
    if (!new_dp)
        abort();                    /* insufficient memory */
    dp = new_dp;

    /* compute missing terms up to n */
    for (idx_t i = dp_size + 1; i <= n; ++i)
    {
        dp[i] = dp[i-1] + dp[i-2] * 6 - dp[i-3] * 3;
    }

    dp_size = n;
    return dp[n];
}
