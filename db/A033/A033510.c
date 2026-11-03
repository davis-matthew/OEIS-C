/*
 * A033510  Number of matchings in graph P_{7} X P_{n}.
 * https://oeis.org/A033510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of matchings in P_7 × P_n. Computed via
 *            transfer‑matrix DP over column states
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 200 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static void init_matrix(seq_t M[128][128])
{
    const unsigned int EDGE_MASK[6] = {
        (1u << 0) | (1u << 1),
        (1u << 1) | (1u << 2),
        (1u << 2) | (1u << 3),
        (1u << 3) | (1u << 4),
        (1u << 4) | (1u << 5),
        (1u << 5) | (1u << 6)
    };
    const unsigned int ALL_BITS = 0x7F;        /* 7 lowest bits */

    for (int s = 0; s < 128; ++s) {
        for (int e = 0; e < 64; ++e) {
            /* horizontal edges must not overlap */
            if (e & (e >> 1)) continue;
            /* edges must not touch a vertex already occupied by S */
            bool ok = true;
            for (int i = 0; i < 6 && ok; ++i) {
                if (e & (1 << i)) {
                    if (s & EDGE_MASK[i]) ok = false;
                }
            }
            if (!ok) continue;

            /* vertices used by horizontal edges */
            unsigned int used = 0;
            for (int i = 0; i < 6; ++i) {
                if (e & (1 << i)) used |= EDGE_MASK[i];
            }

            unsigned int free = (~(s | used)) & ALL_BITS;

            /* every subset of free vertices may become the next state */
            unsigned int sub = free;
            for (;;) {
                M[s][sub] += 1;
                if (sub == 0) break;
                sub = (sub - 1) & free;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    static seq_t M[128][128];
    static bool initialized = false;
    if (!initialized) {
        init_matrix(M);
        initialized = true;
    }

    seq_t dp[128]  = {0};
    seq_t ndp[128] = {0};
    dp[0] = 1;

    for (idx_t col = 0; col < n; ++col) {
        /* reset ndp */
        for (int i = 0; i < 128; ++i) ndp[i] = 0;

        for (int s = 0; s < 128; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            /* iterate over all possible next states */
            for (int t = 0; t < 128; ++t) {
                seq_t w = M[s][t];
                if (w == 0) continue;
                ndp[t] = ndp[t] + val * w;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 128; ++i) dp[i] = ndp[i];
    }

    return dp[0];
}
