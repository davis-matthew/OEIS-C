/*
 * A239399  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×2 arrays with elements 0‒3 that never
 *            equal * (1 + sum of left elements) mod 4 or (2 + sum of above
 *            elements) mod 4. * The implementation uses dynamic programming
 *            over rows; the state is the * pair of column sums modulo 4 after
 *            the previous row. Each transition * correspon...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* defined only for n≥1, but guard anyway */
        return (seq_t)0;

    /* Transition matrix: precomputed once.  If trans[p][q]==1 then a
     * row can take the array from previous sums 'p' to new sums 'q'.         */
    static unsigned char trans[16][16] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (int prev = 0; prev < 16; ++prev) {
            int s0 = prev / 4;        /* previous column‑0 sum modulo 4   */
            int s1 = prev % 4;        /* previous column‑1 sum modulo 4   */
            for (int v0 = 0; v0 < 4; ++v0) {
                /* constraints for column 0 */
                if (v0 == 1) continue;                     /* left element          */
                if (v0 == (s0 + 2) % 4) continue;          /* above element         */
                for (int v1 = 0; v1 < 4; ++v1) {
                    /* constraints for column 1 */
                    if (v1 == (v0 + 1) % 4) continue;       /* sum to the left       */
                    if (v1 == (s1 + 2) % 4) continue;       /* above element         */
                    int ns0 = (s0 + v0) % 4;                /* new column‑0 sum      */
                    int ns1 = (s1 + v1) % 4;                /* new column‑1 sum      */
                    int nxt = ns0 * 4 + ns1;
                    trans[prev][nxt] = 1;                   /* unique mapping        */
                }
            }
        }
        initialized = true;
    }

    /* DP vectors for successive rows */
    seq_t dp[16]     = {0};
    seq_t nextdp[16] = {0};

    /* First row – start from the implicit zero state (0,0) */
    const int zero_state = 0;
    for (int s = 0; s < 16; ++s)
        if (trans[zero_state][s])
            dp[s] = (seq_t)1;

    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < 16; ++s)
            total += dp[s];
        return total;
    }

    /* Subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            nextdp[i] = (seq_t)0;
        for (int prev = 0; prev < 16; ++prev) {
            seq_t ways = dp[prev];
            if (!ways) continue;
            for (int nxt = 0; nxt < 16; ++nxt)
                if (trans[prev][nxt])
                    nextdp[nxt] += ways;
        }
        for (int i = 0; i < 16; ++i)
            dp[i] = nextdp[i];
    }

    /* Sum over all possible final states */
    seq_t total = 0;
    for (int s = 0; s < 16; ++s)
        total += dp[s];
    return total;
}
