/*
 * A116378  Number of partitions of n into parts with digital root = 8.
 * https://oeis.org/A116378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts whose digital root is 8
   (parts are exactly the positive integers ≡ 8 (mod 9) ).
   The routine implements the standard unbounded‑coin DP:
       dp[0] = 1
       for each part p = 8 + 9k (p ≤ n)
           for v = p .. n
               dp[v] += dp[v - p]
   The result is dp[n].
   Complexity: O(n²/9) which is below 2 s for n ≈ 10 000 on modern
   hardware.  All arithmetic is performed in the seq_t type as
   required.  The function is safe for any order of calls. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n) {
    /* a(1) is the first term; for n < 1 the sequence is undefined, return 0 */
    if (n < 1) return (seq_t)0;

    /* Allocate DP table of size n+1, zero‑initialised. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure – unlikely in tests */

    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i) dp[i] = (seq_t)0;

    /* Coin change DP over parts congruent to 8 mod 9 */
    for (idx_t p = 8; p <= n; p += 9) {
        for (idx_t v = p; v <= n; ++v) {
            dp[v] = dp[v] + dp[v - p];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
