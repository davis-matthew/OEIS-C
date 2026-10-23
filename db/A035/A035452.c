/*
 * A035452  Number of partitions of n into parts 8k+1 or 8k+6.
 * https://oeis.org/A035452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 153, u32 n <= 467, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035452 – Number of partitions of n into parts 8k+1 or 8k+6.
    Computation uses dynamic programming:
        dp[0] = 1
        dp[i] = Σ  dp[i-p]   (sum over p≤i with p≡1 or 6 (mod 8))
    dp[n] is the desired value. */
/*  This routine is valid C11 and C++17.  It stores a static table that
    is grown as needed.  No floating point is used; only integer
    operations are employed, so the code works for the arbitrary‑
    precision build as well. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table for dynamic programming; grows as required */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;

    if (n == 0) {
        /* The sequence is defined for n ≥ 1; return 0 for safety. */
        return (seq_t)0;
    }

    /* Resize table if it is too small.  Cast of realloc is needed
       for C++ compatibility. */
    if (dp_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 (caller may handle). */
            return (seq_t)0;
        }
        dp = tmp;
        dp_cap = n + 1;
    }

    /* Initialise dp[0] = 1; dp[1..n] = 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }

    /* Dynamic programming: add contributions of each allowed part. */
    for (idx_t p = 1; p <= n; ++p) {
        if (p % 8 == 1 || p % 8 == 6) {
            for (idx_t j = p; j <= n; ++j) {
                dp[j] += dp[j - p];
            }
        }
    }

    return dp[n];
}
