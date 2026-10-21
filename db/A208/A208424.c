/*
 * A208424  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays that avoid * horizontal
 *            patterns 000 and 001, and vertical patterns 000 and 011. *
 *            Computed by dynamic programming over 7‑bit column states. *
 *            a(1)=25, a(2)=625, a(3)=2625, …
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute allowed 7‑bit column masks and adjacency table. */
    static uint8_t masks[128];
    static unsigned char trans[128][128];
    static size_t K = 0;
    static bool inited = false;

    if (!inited) {
        /* Determine masks that contain no vertical 000 or 011. */
        K = 0;
        for (int m = 0; m < 128; ++m) {
            bool ok = true;
            for (int r = 0; r <= 4; ++r) {
                int a = (m >> r) & 1;
                int b = (m >> (r + 1)) & 1;
                int c = (m >> (r + 2)) & 1;
                if ((a == 0 && b == 0 && c == 0) ||
                    (a == 0 && b == 1 && c == 1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                masks[K++] = (uint8_t)m;
            }
        }

        /* Transition rule: forbid two consecutive zeros horizontally. */
        for (size_t i = 0; i < K; ++i) {
            uint8_t mi = masks[i];
            for (size_t j = 0; j < K; ++j) {
                uint8_t mj = masks[j];
                if ((~mi & ~mj & 0x7F) == 0) {
                    trans[i][j] = 1;   /* allowed transition */
                } else {
                    trans[i][j] = 0;   /* forbidden */
                }
            }
        }
        inited = true;
    }

    /* Handle the first two columns separately (no horizontal constraint). */
    if (n == 1) {
        return (seq_t)K;
    }
    if (n == 2) {
        return (seq_t)K * (seq_t)K;
    }

    /* Dynamic programming for n ≥ 3.  dp[i] = number of ways to
       obtain a matrix of length (step–1) ending with column mask i. */
    seq_t dp[128];
    seq_t ndp[128];
    for (size_t i = 0; i < K; ++i) {
        dp[i] = (seq_t)K;   /* length 2: first column can be any mask */
    }

    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < K; ++i) {
            ndp[i] = (seq_t)0;
        }
        for (size_t i = 0; i < K; ++i) {
            seq_t ways = dp[i];
            if (ways == (seq_t)0) continue;
            for (size_t j = 0; j < K; ++j) {
                if (trans[i][j]) {
                    ndp[j] = ndp[j] + ways;
                }
            }
        }
        for (size_t i = 0; i < K; ++i) {
            dp[i] = ndp[i];
        }
    }

    /* Sum all ways for the final column. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < K; ++i) {
        result = result + dp[i];
    }
    return result;
}
