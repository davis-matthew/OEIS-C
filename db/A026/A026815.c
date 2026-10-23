/*
 * A026815  Number of partitions of n in which the greatest part is 9.
 * https://oeis.org/A026815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with greatest part 9
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 285, u64 n <= 4760,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the coefficient of x^(n-9) in 1/∏_{k=1}^9 (1-x^k). */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;      /* dp[s] = number of partitions of s using parts 1..9 */
static size_t dp_cap = 0;     /* capacity of dp[] (size in elements) */

/* Ensure dp[] is computed up to sum = target */
static void ensure_dp(size_t target)
{
    size_t need = target + 1;
    if (need <= dp_cap)
        return;

    /* Allocate space for all required sums */
    seq_t *newdp = (seq_t *)realloc(dp, need * sizeof(seq_t));
    if (!newdp)
        exit(EXIT_FAILURE);

    /* Initialize all entries to 0 and set dp[0] = 1 */
    for (size_t i = 0; i < need; ++i)
        newdp[i] = 0;
    newdp[0] = 1;

    /* Compute partitions using parts 1..9 incrementally */
    for (int k = 1; k <= 9; ++k) {
        for (size_t i = k; i < need; ++i)
            newdp[i] += newdp[i - k];
    }

    dp = newdp;
    dp_cap = need;
}

seq_t a(idx_t n)
{
    if (n < 9)
        return (seq_t)0;

    size_t target = (size_t)(n - 9);
    ensure_dp(target);
    return dp[target];
}
