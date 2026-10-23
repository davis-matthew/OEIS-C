/*
 * A131364  Number of partitions of n into distinct repdigits of digits of n.
 * https://oeis.org/A131364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into distinct repdigits of
 *            digits of n. The algorithm enumerates all repdigits that use a
 *            digit appearing in n, and counts the subsets that sum exactly to
 *            n via a classic 0‑1 knapsack DP
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    /* Determine which decimal digits occur in n */
    bool has_digit[10] = {0};
    if (n == 0) has_digit[0] = true;   /* not used for repdigits, but kept for completeness */
    idx_t tmp = n;
    while (tmp > 0) {
        has_digit[tmp % 10] = true;
        tmp /= 10;
    }

    /* Collect all repdigits composed of those digits, up to n */
    seq_t items[40]; /* at most 9 digits * 4 repdigits = 36 items for n <= 10000, safe margin */
    size_t m = 0;

    for (int d = 1; d <= 9; ++d) {          /* digit 0 never yields a non‑zero repdigit */
        if (!has_digit[d])
            continue;
        seq_t val = (seq_t)d;
        while (val <= n) {
            items[m++] = val;
            val = val * (seq_t)10 + (seq_t)d;
        }
    }

    /* 0‑1 subset sum DP: dp[s] = number of ways to obtain sum s */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof *dp);
    if (!dp)
        return (seq_t)0;                     /* Out of memory – return 0 as safe default */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    for (size_t i = 0; i < m; ++i) {
        idx_t weight = (idx_t)items[i];     /* weight fits in idx_t because items[i] <= n */
        for (idx_t s = n; s >= weight; --s)
            dp[s] += dp[s - weight];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
