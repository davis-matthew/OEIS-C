/*
 * A223678  Number of nX6 0..1 arrays with rows and antidiagonals unimodal.
 * https://oeis.org/A223678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements a dynamic‑programming algorithm to count
 * the number of n×6 binary arrays whose rows and anti‑diagonals
 * are unimodal (A223678, offset 1).  
 * 
 * The DP keeps only the states of the last five anti‑diagonals,
 * because each diagonal has at most six bits, so after the first
 * six rows the remaining diagonals never change again.
 * The state is encoded as a base‑3 number (0, 1, 2 represent the
 * phase of a diagonal).  22 valid 6‑bit patterns are enumerated and
 * pre‑computed; a transition table gives the next state for each
 * current state and pattern.
 * 
 * Complexity: O(n · 243 · 22) ≈ 6.8·10⁴ operations per row, well
 * below the required time limit for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Pre‑computation ------------------------------------------------- */
    static bool initialized = false;
    static int pow3[5];                                   /* 3^i for i=0..4 */
    static int pattern_bits[22][6];
    static int trans[243][22];

    if (!initialized) {
        /* powers of 3 */
        pow3[0] = 1;
        for (int i = 1; i < 5; ++i) pow3[i] = pow3[i-1] * 3;

        /* enumerate the 22 unimodal 6‑bit patterns */
        int pc = 0;
        for (int mask = 0; mask < 64; ++mask) {
            bool seen1 = false, decreasing = false, ok = true;
            for (int j = 0; j < 6; ++j) {
                int bit = (mask >> j) & 1;
                if (!seen1) {
                    if (bit) seen1 = true;
                } else {
                    if (!decreasing) {
                        if (!bit) decreasing = true;
                    } else {
                        if (bit) { ok = false; break; }
                    }
                }
            }
            if (ok) {
                for (int j = 0; j < 6; ++j) pattern_bits[pc][j] = (mask >> j) & 1;
                ++pc;
            }
        }

        /* build transition table trans[state][pattern] */
        for (int s = 0; s < 243; ++s) {
            int phase[5];
            int tmp = s;
            for (int i = 0; i < 5; ++i) {
                phase[i] = tmp % 3;
                tmp /= 3;
            }

            for (int r = 0; r < 22; ++r) {
                int upd[6];
                bool ok = true;

                /* diagonals 0..4 */
                for (int j = 0; j < 5; ++j) {
                    int cur = phase[j];
                    int bit = pattern_bits[r][j];
                    int nxt;
                    if (cur == 0)            nxt = bit ? 1 : 0;
                    else if (cur == 1)       nxt = bit ? 1 : 2;
                    else /* cur==2 */        nxt = (bit == 0) ? 2 : -1;
                    if (nxt == -1) { ok = false; break; }
                    upd[j] = nxt;
                }
                if (!ok) {
                    trans[s][r] = -1;
                    continue;
                }

                /* new diagonal (index 5) starts from undefined */
                { int bit = pattern_bits[r][5]; upd[5] = bit ? 1 : 0; }

                /* encode next state (phases[1]..phases[5]) */
                int ns = 0;
                for (int k = 0; k < 5; ++k) ns += upd[k+1] * pow3[k];
                trans[s][r] = ns;
            }
        }
        initialized = true;
    }

    /* Special case n == 0 ------------------------------------------------ */
    if (n == 0) return 0;

    /* DP arrays ---------------------------------------------------------- */
    static seq_t dp[243];
    static seq_t ndp[243];

    /* initialise dp to zero */
    for (int i = 0; i < 243; ++i) dp[i] = 0;

    /* first row: start from all undefined diagonals */
    for (int r = 0; r < 22; ++r) {
        int upd[6];
        for (int j = 0; j < 6; ++j) {
            int bit = pattern_bits[r][j];
            upd[j] = bit ? 1 : 0;
        }
        int ns = 0;
        for (int k = 0; k < 5; ++k) ns += upd[k+1] * pow3[k];
        dp[ns] += 1;
    }

    /* process remaining rows --------------------------------------------- */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 243; ++i) ndp[i] = 0;

        for (int s = 0; s < 243; ++s) {
            if (dp[s] == 0) continue;
            seq_t val = dp[s];
            for (int r = 0; r < 22; ++r) {
                int ns = trans[s][r];
                if (ns >= 0) ndp[ns] += val;
            }
        }

        for (int i = 0; i < 243; ++i) dp[i] = ndp[i];
    }

    /* sum over all states ----------------------------------------------- */
    seq_t total = 0;
    for (int i = 0; i < 243; ++i) total += dp[i];
    return total;
}
