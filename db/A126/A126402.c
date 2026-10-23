/*
 * A126402  Number of base 15 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126402: Number of base‑15 n‑digit numbers whose adjacent
 * digits differ by at most two.
 *
 * The sequence is computed by dynamic programming:
 *   dp[1][d] = 1   for all digits d in {0,…,14}
 *   dp[k+1][d] = Σ dp[k][j]  over all j with |d-j| ≤ 2
 *   a(n) = Σ dp[n][d]   (for n ≥ 1),  a(0) = 1.
 *
 * The implementation uses only addition on seq_t values and works
 * for all builds where seq_t is an unsigned integer type or an
 * arbitrary‑precision integer class.  No other library functions
 * or floating‑point operations are required. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case: empty string. */
    if (n == 0) return (seq_t)1;

    /* dp arrays for two successive steps.  Index 0 corresponds to
     * digit 0 (i.e., digit 1 in base‑15 counting). */
    seq_t curr[15];
    seq_t next[15];

    /* Initialise for length 1. */
    for (idx_t i = 0; i < 15; ++i) curr[i] = (seq_t)1;

    /* If n == 1, the answer is simply 15. */
    if (n == 1) {
        seq_t total = 0;
        for (idx_t i = 0; i < 15; ++i) total += curr[i];
        return total;
    }

    /* Iterate from length 2 up to length n. */
    for (idx_t step = 2; step <= n; ++step) {
        for (idx_t d = 0; d < 15; ++d) {
            seq_t sum = 0;
            idx_t lo = (d >= 2) ? d - 2 : 0;
            idx_t hi = (d + 2 < 15) ? d + 2 : 14;
            for (idx_t j = lo; j <= hi; ++j) {
                sum += curr[j];
            }
            next[d] = sum;
        }
        /* Copy next into curr for the next iteration. */
        for (idx_t d = 0; d < 15; ++d) curr[d] = next[d];
    }

    /* Sum all ending‑digit counts to obtain a(n). */
    seq_t total = 0;
    for (idx_t i = 0; i < 15; ++i) total += curr[i];
    return total;
}
