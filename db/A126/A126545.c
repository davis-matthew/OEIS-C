/*
 * A126545  Number of base 24 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of n‑digit base‑24 numbers whose adjacent digits differ by at most 5.
   Computed by dynamic programming on the 24×24 adjacency matrix. */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 (empty number). */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Constants: base 24, adjacency window ±5. */
    const idx_t B = 24;   /* number of digits (0–23) */
    const idx_t K = 5;    /* maximum allowed difference */

    /* DP arrays for the previous (len-1) and current (len) digit counts. */
    seq_t prev[B];
    seq_t cur[B];

    /* Initialise dp for length 1: every digit (including 0) can start the number. */
    for (idx_t i = 0; i < B; ++i) {
        prev[i] = (seq_t)1;
    }

    /* Extend DP up to length n. */
    for (idx_t len = 2; len <= n; ++len) {
        /* Compute dp for all digits at this length. */
        for (idx_t j = 0; j < B; ++j) {
            seq_t s = (seq_t)0;
            idx_t low  = (j >= K) ? j - K : 0;
            idx_t high = (j + K < B) ? j + K : B - 1;
            for (idx_t k = low; k <= high; ++k) {
                s += prev[k];
            }
            cur[j] = s;
        }
        /* Move current counts to prev for next iteration. */
        for (idx_t j = 0; j < B; ++j) {
            prev[j] = cur[j];
        }
    }

    /* Sum all ways for the final length to obtain a(n). */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < B; ++i) {
        total += prev[i];
    }
    return total;
}
