/*
 * A183318  Number of nX6 binary arrays with an element zero only if there are
 *          an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Main transition‑based implementation: a(n) counts the number of
 * n×6 binary arrays in which each zero has an even number of ones
 * to its left (in the same row) and an even number of ones above it.
 * The algorithm uses a dynamic‑programming state that records, for
 * each column, the parity of ones seen so far.  For each row a
 * valid 6‑bit mask is chosen such that all zeros respect the left‑
 * parity condition, and for the columns where a zero is placed the
 * vertical parity must be even.  The state is updated by XORing
 * with the chosen mask.  After processing n rows the sum over all
 * possible final parity states gives a(n). */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---- 0 rows: empty matrix ---- */
    if (n == (idx_t)0) return (seq_t)0;

    /* Tables produced only once */
    static bool ready = false;
    static unsigned char left_ok[64];
    static unsigned char trans[64][64];
    static uint8_t trans_len[64];

    if (!ready) {
        /* Pre‑compute masks that satisfy the left‑parity rule. */
        for (int mask = 0; mask < 64; ++mask) {
            int ones = 0;
            bool ok = true;
            for (int j = 0; j < 6; ++j) {
                if (mask & (1 << j))
                    ++ones;
                else if ((ones & 1) != 0) {
                    ok = false;
                    break;
                }
            }
            left_ok[mask] = ok ? 1 : 0;
        }

        /* For every current column parity state, list all masks that can
           be appended in the next row. */
        for (int p = 0; p < 64; ++p) {
            int len = 0;
            for (int mask = 0; mask < 64; ++mask) {
                if (!left_ok[mask]) continue;
                bool ok = true;
                for (int j = 0; j < 6; ++j) {
                    /* If current cell is zero, the vertical parity must
                       be even. */
                    if ((mask & (1 << j)) == 0) {
                        if (p & (1 << j)) { ok = false; break; }
                    }
                }
                if (ok) {
                    trans[p][len++] = (unsigned char)mask;
                }
            }
            trans_len[p] = (uint8_t)len;
        }
        ready = true;
    }

    static seq_t dp[64];
    static seq_t nextdp[64];

    /* Initialise with empty history (no rows yet). */
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each row. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) nextdp[i] = (seq_t)0;

        for (int p = 0; p < 64; ++p) {
            seq_t val = dp[p];
            if (val == (seq_t)0) continue;
            uint8_t len = trans_len[p];
            for (int k = 0; k < len; ++k) {
                int q = p ^ trans[p][k];
                nextdp[q] += val;
            }
        }

        /* Move to the next row. */
        for (int i = 0; i < 64; ++i) dp[i] = nextdp[i];
    }

    /* Sum over all possible final parity states. */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 64; ++i) ans += dp[i];
    return ans;
}
