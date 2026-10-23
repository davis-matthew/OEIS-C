/*
 * A035432  Number of partitions of n into parts 7k+2 or 7k+4.
 * https://oeis.org/A035432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of partitions of n into parts of the form
 *            7k+2 or 7k+4
 * Range:     u8 n <= 55, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n≥1.  For n<1 this function returns 0.  */

/* The implementation constructs the list of admissible parts up to n
   and then uses the classical coin‑change DP routine
      dp[0] = 1
      for each part p:
          for i = p .. n:
              dp[i] += dp[i-p]
   This yields the desired partition count for any n with
   reasonable performance for the required range (up to ~10 000).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* offset is 1 */

    /* Build the list of allowed parts (7k+2 and 7k+4) ≤ n. */
    size_t parts_cnt = 0;
    idx_t k = 0;
    while (true) {
        uint64_t p2 = 7ULL * (uint64_t)k + 2;
        uint64_t p4 = 7ULL * (uint64_t)k + 4;
        if (p2 > n && p4 > n) break;
        if (p2 <= n) parts_cnt++;
        if (p4 <= n) parts_cnt++;
        k++;
    }

    uint64_t *parts = NULL;
    if (parts_cnt > 0) {
        parts = (uint64_t *)malloc(parts_cnt * sizeof(uint64_t));
        if (!parts) abort();               /* allocation failure */
        size_t idx = 0;
        k = 0;
        while (true) {
            uint64_t p2 = 7ULL * (uint64_t)k + 2;
            uint64_t p4 = 7ULL * (uint64_t)k + 4;
            if (p2 > n && p4 > n) break;
            if (p2 <= n) parts[idx++] = p2;
            if (p4 <= n) parts[idx++] = p4;
            k++;
        }
    }

    /* Allocate DP array. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) abort();

    /* Initialise dp[0] and zero others. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Coin change DP. */
    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        uint64_t p = parts[pi];
        for (idx_t i = (idx_t)p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];

    free(dp);
    free(parts);

    return result;
}
