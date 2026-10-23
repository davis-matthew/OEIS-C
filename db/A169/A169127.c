/*
 * A169127  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=18. Let f_r[n] be the number of reduced words of
 *            length n whose last alternating run between distinct letters has
 *            length r (2 ≤ r ≤ 25). Base: f_2[2] = 18·17, f_r[2] = 0 for r>2.
 *            Recurrence (for n>2): f_{r+1}[n] += f_r[n-1] (for 2 ≤ r ≤ 24)
 *            f_2[n] += f_r[n-1]·(18-2) (for 2 ≤ r ≤ 25)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

seq_t a(idx_t n)
{
    const seq_t K          = (seq_t)18;   /* number of generators */
    const idx_t MAX_RUN    = 25;          /* maximal allowed run length */
    const seq_t PREV_DIFF  = K - 2;       /* 16 */

    if (n == 0) return (seq_t)1;
    if (n == 1) return K;

    /* f[r] holds numbers for current length, indices 2..25 are used. */
    seq_t f[26];
    seq_t next[26];
    for (idx_t i = 0; i < 26; ++i) f[i] = (seq_t)0;
    f[2] = K * ((seq_t)17);              /* 18·17 = 306 for n = 2 */

    if (n == 2) {
        return f[2];
    }

    /* Compute for lengths 3..n. */
    for (idx_t len = 3; len <= n; ++len) {
        for (idx_t i = 0; i < 26; ++i) next[i] = (seq_t)0;

        /* r = 2 .. 24 : can extend to r+1 and start a new run of length 2. */
        for (idx_t r = 2; r <= 24; ++r) {
            seq_t old = f[r];
            if (!old) continue;
            next[r + 1] += old;                /* extend alternation */
            next[2]     += old * PREV_DIFF;    /* start new run with other letter */
        }

        /* r = 25 : cannot extend to 26. */
        seq_t old25 = f[25];
        if (old25) {
            next[2] += old25 * PREV_DIFF;
        }

        /* Prepare for next iteration. */
        for (idx_t r = 2; r <= MAX_RUN; ++r) f[r] = next[r];
    }

    /* Sum all remaining states for the final answer. */
    seq_t total = (seq_t)0;
    for (idx_t r = 2; r <= MAX_RUN; ++r) total += f[r];
    return total;
}
