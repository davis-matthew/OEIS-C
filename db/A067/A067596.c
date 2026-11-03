/*
 * A067596  Number of partitions of n into palindrome parts in base 2.
 * https://oeis.org/A067596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of partitions of n into parts whose * binary
 *            representation is a palindrome. The function uses a * simple
 *            dynamic‑programming coin‑change algorithm
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Dynamically allocate the DP table and the list of palindromic parts. */
    size_t size = (size_t)(n + 1);
    seq_t *dp = mem_new(seq_t, size);
    if (!dp) return 0;  /* allocation failure, unlikely for n≤10000 */

    /* Initialize dp[0] = 1, the empty partition. */
    for (size_t i = 0; i < size; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Collect all binary palindromic numbers ≤ n. */
    seq_t *coins = mem_new(seq_t, size);
    if (!coins) { mem_free(dp); return 0; }
    size_t coin_count = 0;

    for (idx_t x = 1; x <= n; ++x)
    {
        if (is_palindrome_base((seq_t)x, 2))
            coins[coin_count++] = (seq_t)x;
    }

    /* Classic coin‑change DP: iterate over coins, then over sums. */
    for (size_t ci = 0; ci < coin_count; ++ci)
    {
        seq_t coin = coins[ci];
        if (coin == 0) continue;  /* safety check */
        idx_t start = (idx_t)coin;
        for (idx_t s = start; s <= n; ++s)
        {
            dp[s] += dp[s - start];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    mem_free(coins);
    return result;
}
