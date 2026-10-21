/*
 * A202903  Number of nX5 0..1 arrays with every one equal to some NW, E or S
 *          neighbor.
 * https://oeis.org/A202903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices where every 1 has at least
 *            one 1 among its North‑West (NW), East (E) or South (S)
 *            neighbors. The implementation uses dynamic programming over row
 *            masks with a state consisting of two consecutive rows' masks
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   requiredSmask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure required headers are available. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/requiredSmask.h"

/* Pre‑computed tables for transitions and final validity. */
static int  allowed_next[32][32];       /* Bitmask of allowed next row masks. */
static bool final_ok[32][32] = {0};     /* Validity of state as final row. */
static bool precomputed = false;

/* Prepare transition tables once. */
static void init_precomputation(void)
{
    if (precomputed) return;
    for (int pp = 0; pp < 32; ++pp) {
        for (int pm = 0; pm < 32; ++pm) {
            int req = requiredSmask(pp, pm);
            /* A state is final‑valid if it requires no S neighbors. */
            final_ok[pp][pm] = (req == 0);
            int mask = 0;
            for (int cn = 0; cn < 32; ++cn) {
                if ((cn & req) == req)          /* cn contains all required bits */
                    mask |= (1 << cn);
            }
            allowed_next[pp][pm] = mask;
        }
    }
    precomputed = true;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    init_precomputation();                /* Ensure tables are ready. */

    /* dp[prevPrev][prev] holds number of ways up to current row. */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];
    memset(dp, 0, sizeof(dp));

    /* First row: prevPrev = 0, prev = curMask. */
    for (int cur = 0; cur < 32; ++cur) {
        dp[0][cur] = 1;
    }

    /* Process rows 2..n (if any). */
    for (idx_t row = 2; row <= n; ++row) {
        memset(newdp, 0, sizeof(newdp));
        for (int pp = 0; pp < 32; ++pp) {
            for (int pm = 0; pm < 32; ++pm) {
                seq_t ways = dp[pp][pm];
                if (!ways) continue;
                int allow = allowed_next[pp][pm];
                /* Enumerate allowed next masks. */
                for (int cn = 0; cn < 32; ++cn) {
                    if (allow & (1 << cn)) {
                        newdp[pm][cn] += ways;
                    }
                }
            }
        }
        /* Swap dp and newdp for next iteration. */
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Sum up all ways that end in a valid final state. */
    seq_t result = 0;
    for (int pp = 0; pp < 32; ++pp) {
        for (int pm = 0; pm < 32; ++pm) {
            if (!dp[pp][pm]) continue;
            if (final_ok[pp][pm])
                result += dp[pp][pm];
        }
    }
    return result;
}
