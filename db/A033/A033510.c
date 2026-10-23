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
 * Helpers:   init_matrix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/init_matrix.h"

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
