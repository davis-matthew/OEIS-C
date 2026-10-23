/*
 * A108485  a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-2*k,2*k)*2^(n-k).
 * https://oeis.org/A108485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)+8*a(n-3)-4*a(n-4), a(0)=1, a(1)=2, a(2)=6,
 *            a(3)=32
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation table for the sequence */
static seq_t *dp = NULL;
static idx_t dp_cap = 0;

/* Initialise the table with the first four terms */
static void init_seq(void)
{
    if (dp == NULL) {
        dp_cap = 4;
        dp = (seq_t *)calloc(dp_cap, sizeof(seq_t));
        if (!dp) exit(EXIT_FAILURE);
        dp[0] = 1;
        dp[1] = 2;
        dp[2] = 6;
        dp[3] = 32;
    }
}

/* Ensure that the table can hold index n */
static void ensure_capacity(idx_t n)
{
    if (dp_cap <= n) {
        idx_t new_cap = dp_cap ? dp_cap * 2 : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *newp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);
        dp = newp;
        dp_cap = new_cap;
    }
}

/* Sequence A108485 */
seq_t a(idx_t n)
{
    init_seq();
    if (n < 4) return dp[n];
    ensure_capacity(n);
    /* Compute terms up to n, if not already present */
    for (idx_t i = 4; i <= n; ++i) {
        dp[i] = 4 * dp[i-1] + 8 * dp[i-3] - 4 * dp[i-4];
    }
    return dp[n];
}
