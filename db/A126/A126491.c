/*
 * A126491  Number of base 23 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126491: Number of base‑23 n‑digit numbers with adjacent digits differing by ≤3.
   Recurrence:  a(0)=1,
                for n≥1:      dp₁[d] = 1   for all d (length 1)
                 for L=2..n:
                     dp_L[d] = Σ_{|k−d|≤3} dp_{L-1}[k]
                 a(n) = Σ_{d=0}^{22} dp_n[d]    */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants */
    const idx_t DIGITS = 23;
    const idx_t MAX_DIFF = 3;

    /* Local DP arrays – fixed size, no VLA */
    seq_t dp[DIGITS];
    seq_t newdp[DIGITS];

    /* n = 0 => only the empty number */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Initialise length‑1 sequences: each digit occurs once */
    for (idx_t d = 0; d < DIGITS; ++d) {
        dp[d] = (seq_t)1;
    }

    /* Build sequences of length 2 … n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < DIGITS; ++d) {
            /* Sum over all previous digits k that are within MAX_DIFF of d */
            seq_t sum = (seq_t)0;
            idx_t low  = (d > MAX_DIFF) ? d - MAX_DIFF : 0;
            idx_t high = (d + MAX_DIFF < DIGITS) ? d + MAX_DIFF : DIGITS - 1;
            for (idx_t k = low; k <= high; ++k) {
                sum += dp[k];
            }
            newdp[d] = sum;
        }
        /* Move newdp into dp for the next iteration */
        for (idx_t d = 0; d < DIGITS; ++d) {
            dp[d] = newdp[d];
        }
    }

    /* Sum over all ending digits to get a(n) */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < DIGITS; ++d) {
        total += dp[d];
    }
    return total;
}
