/*
 * A207509  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by DP over the six admissible row patterns * (all
 *            binary strings of length 3 except 000 and 011). Each transition
 *            * enforces that for any column, if the two upper bits are equal,
 *            * the third must be 0 (avoiding the vertical patterns 001 and
 *            111). * Initial values: a(1)=6, a(2)=36, a...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 33, u64 n <= 72, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* admissible row patterns in binary */
    static const int rows[6] = {1, 2, 4, 5, 6, 7};

    /* offset handling: the sequence starts at n=1 */
    if (n == 0) return (seq_t)0;   /* unused, but defined for safety */
    if (n == 1) return (seq_t)6;   /* six single rows */
    if (n == 2) return (seq_t)36;  /* 6 × 6 pairs */

    /* dp[i][j] counts ways ending with pattern rows[i] (previous‑previous)
     * and rows[j] (previous).  All values are of type seq_t.          */
    seq_t dp[6][6];
    seq_t ndp[6][6];

    /* initialise for length 2: every ordered pair is allowed */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            dp[i][j] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* zero next state */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                ndp[i][j] = (seq_t)0;

        /* extend with a new row k */
        for (int i = 0; i < 6; ++i) {
            int a = rows[i];
            for (int j = 0; j < 6; ++j) {
                seq_t cnt = dp[i][j];
                if (cnt == (seq_t)0) continue;
                int b = rows[j];
                int eqmask = (~(a ^ b)) & 7;          /* bits where a==b */
                for (int k = 0; k < 6; ++k) {
                    int c = rows[k];
                    /* valid if no 1 in c at positions where a==b */
                    if ((c & eqmask) == 0)
                        ndp[j][k] += cnt;
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all ways for the last two rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            total += dp[i][j];

    return total;
}
