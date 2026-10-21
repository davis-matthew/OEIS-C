/*
 * A220641  Number of ways to reciprocally link elements of an n X 5 array
 *          either to themselves or to exactly one king-move neighbor.
 * https://oeis.org/A220641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of reciprocal linkings in an n×5 king graph. A
 *            transfer‑matrix DP over 5‑bit column states is used
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   gen_trans_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are supplied by the build. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gen_trans_bits.h"

static uint32_t trans[32][32];
static int trans_precomputed = 0;

/* Precompute the 32×32 transition table once. */
static void precompute_transitions(void)
{
    if (trans_precomputed) return;
    for (int sPrev = 0; sPrev < 32; ++sPrev) {
        uint32_t cnt[32] = {0};
        uint8_t freeMask = (uint8_t)(~(uint8_t)sPrev) & 0x1F; /* free vertices */
        gen_trans_bits(freeMask, 0, cnt);
        for (int sNext = 0; sNext < 32; ++sNext) {
            trans[sPrev][sNext] = cnt[sNext];
        }
    }
    trans_precomputed = 1;
}

seq_t a(idx_t n)
{
    precompute_transitions();

    seq_t dp[32] = {0}, next[32] = {0};
    dp[0] = (seq_t)1;  /* start with no blocked vertices */

    for (idx_t col = 0; col < n; ++col) {
        /* clear next array */
        for (int i = 0; i < 32; ++i) next[i] = (seq_t)0;

        for (int sPrev = 0; sPrev < 32; ++sPrev) {
            if (dp[sPrev] == (seq_t)0) continue;
            for (int sNext = 0; sNext < 32; ++sNext) {
                uint32_t ways = trans[sPrev][sNext];
                if (ways == 0) continue;
                next[sNext] = next[sNext] + (dp[sPrev] * (seq_t)ways);
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 32; ++i) dp[i] = next[i];
    }

    /* after last column, no pending edges to a non‑existent column */
    return dp[0];
}
