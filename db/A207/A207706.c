/*
 * A207706  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n binary arrays avoiding the patterns
 *   000 and 010 horizontally, and 001 and 110 vertically.
 *
 * The implementation uses a breadth‑first DP over column states.
 * Each state stores the last two columns (as 5‑bit masks).
 * Because there are only 32 possible column patterns, the DP
 * has 1024 states.  The transitions are precomputed as bit masks
 * to keep the per‑column cost low.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute vertical‑valid columns. */
    static bool vertValid[32];
    static bool vertValidComputed = false;
    if (!vertValidComputed) {
        for (int col = 0; col < 32; ++col) {
            bool ok = true;
            for (int r = 0; r < 3 && ok; ++r) {
                int a = (col >> r)     & 1;
                int b = (col >> (r+1)) & 1;
                int c = (col >> (r+2)) & 1;
                /* forbidden 001 or 110 in consecutive rows */
                if ((a == 0 && b == 0 && c == 1) ||
                    (a == 1 && b == 1 && c == 0))
                    ok = false;
            }
            vertValid[col] = ok;
        }
        vertValidComputed = true;
    }

    /* Count vertical‑valid columns. */
    static idx_t validCount = 0;
    if (validCount == 0) {
        for (int col = 0; col < 32; ++col)
            if (vertValid[col]) ++validCount;
    }

    /* For the special cases n = 1 and n = 2. */
    if (n == 0)  /* offset is 1, no value requested; return 0 */
        return 0;
    if (n == 1)
        return (seq_t)validCount;
    if (n == 2)
        return (seq_t)validCount * (seq_t)validCount;

    /* Precompute transition masks: transMask[p][q] has bits for allowed r. */
    static uint32_t transMask[32][32];
    static bool transComputed = false;
    if (!transComputed) {
        for (int p = 0; p < 32; ++p) {
            if (!vertValid[p]) continue;
            for (int q = 0; q < 32; ++q) {
                if (!vertValid[q]) continue;
                uint32_t mask = 0;
                for (int r = 0; r < 32; ++r) {
                    if (!vertValid[r]) continue;
                    bool ok = true;
                    for (int row = 0; row < 5 && ok; ++row) {
                        int a = (p >> row) & 1;
                        int b = (q >> row) & 1;
                        int c = (r >> row) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 0))
                            ok = false;
                    }
                    if (ok)
                        mask |= (1U << r);
                }
                transMask[p][q] = mask;
            }
        }
        transComputed = true;
    }

    /* DP arrays: dp[p][q] = number of arrays ending with columns p,q */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];

    /* Initialize for column 2 (after processing two columns). */
    for (int p = 0; p < 32; ++p)
        for (int q = 0; q < 32; ++q)
            dp[p][q] = 0;
    for (int p = 0; p < 32; ++p) if (vertValid[p])
        for (int q = 0; q < 32; ++q) if (vertValid[q])
            dp[p][q] = 1;  /* each pair is a valid first two columns */

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero newdp */
        for (int p = 0; p < 32; ++p)
            for (int q = 0; q < 32; ++q)
                newdp[p][q] = 0;

        for (int p = 0; p < 32; ++p) {
            if (!vertValid[p]) continue;
            for (int q = 0; q < 32; ++q) {
                if (!vertValid[q]) continue;
                seq_t ways = dp[p][q];
                if (ways == 0) continue;
                uint32_t mask = transMask[p][q];
                /* iterate set bits of mask */
                while (mask) {
                    int r = __builtin_ctz(mask);
                    mask &= mask - 1;
                    newdp[q][r] += ways;
                }
            }
        }
        /* copy newdp to dp */
        for (int p = 0; p < 32; ++p)
            for (int q = 0; q < 32; ++q)
                dp[p][q] = newdp[p][q];
    }

    /* Sum all DP states for final answer */
    seq_t total = 0;
    for (int p = 0; p < 32; ++p) if (vertValid[p])
        for (int q = 0; q < 32; ++q) if (vertValid[q])
            total += dp[p][q];

    return total;
}
