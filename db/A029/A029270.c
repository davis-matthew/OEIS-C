/*
 * A029270  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts {3, 4, 10, 12}
 * Range:     u8 n <= 115, u16 n <= 811, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation recomputes the dynamic programming table up to n
   on each call.  It is O(n) per call and works for any seq_t and
   idx_t types.  */
#include <stdint.h>

#define MAX_N 20000  /* sufficient for n up to about 10 000 */

static const idx_t PARTS[4] = {3, 4, 10, 12};

seq_t a(idx_t n)
{
    /* The sequence has offset 0, so a(0) = 1. */
    if (n == 0)
        return 1;

    /* Clamp to the maximum supported size.  The problem statement
       guarantees n is at most ~10 000, which is below MAX_N. */
    if (n > MAX_N)
        n = MAX_N;

    /* Local dynamic‑programming table.  We use a static array so that
       the code compiles in both C and C++ and so that seq_t objects
       are properly constructed (in C++). */
    static seq_t dp[MAX_N + 1];
    /* Reset only the used portion of the table.  This keeps the code
       simple and guarantees correctness regardless of previous calls. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Unbounded knapsack style DP: for each allowed part, add
       contributions to all sums ≥ part. */
    for (int p = 0; p < 4; ++p) {
        idx_t part = PARTS[p];
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    return dp[n];
}
