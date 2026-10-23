/*
 * A187722  Number of (n+1)X3 binary arrays with no 2X2 subblock commuting
 *          with any of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A187722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   mul_2x2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of (n+1)×3 binary arrays with no 2×2 subblock */
#include <stdbool.h>
#include "../utils/mul_2x2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Static data that is computed once. */
    static bool init = false;
    static unsigned char L[64];          /* left 2×2 block for each pair of rows */
    static unsigned char R[64];          /* right 2×2 block for each pair of rows */
    static unsigned char comm[16][16];   /* commute table for original 4‑bit matrices */
    static unsigned char adj[64][64];    /* adjacency from previous pair to new pair */

    if (!init)
    {
        /* Pre‑compute commutation table for 16 binary 2×2 matrices. */
        for (int A = 0; A < 16; ++A)
            for (int B = 0; B < 16; ++B)
                comm[A][B] = (mul_2x2(A, B) == mul_2x2(B, A));

        /* Pre‑compute L and R for each pair of rows (a,b). */
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
            {
                int idx = a * 8 + b;
                /* bits of row a */
                unsigned int a0 = (a >> 0) & 1u;
                unsigned int a1 = (a >> 1) & 1u;
                unsigned int a2 = (a >> 2) & 1u;
                /* bits of row b */
                unsigned int b0 = (b >> 0) & 1u;
                unsigned int b1 = (b >> 1) & 1u;
                unsigned int b2 = (b >> 2) & 1u;

                /* left subblock uses columns 0 and 1 */
                L[idx] = (a0 << 3) | (a1 << 2) | (b0 << 1) | b1;
                /* right subblock uses columns 1 and 2 */
                R[idx] = (a1 << 3) | (a2 << 2) | (b1 << 1) | b2;
            }

        /* Pre‑compute adjacency (valid transitions) */
        for (int sp = 0; sp < 64; ++sp)
            for (int sn = 0; sn < 64; ++sn)
            {
                /* Middle row must match: second row of sp equals first row of sn */
                int sp_b = sp & 7;      /* lower 3 bits of sp are row b */
                int sn_a = sn >> 3;     /* upper 3 bits of sn are row a */
                if (sp_b != sn_a) { adj[sp][sn] = 0; continue; }

                /* Horizontal constraint on the new pair. */
                if (comm[L[sn]][R[sn]]) { adj[sp][sn] = 0; continue; }

                /* Vertical constraints between the two 2×2 blocks of the same column. */
                if (comm[L[sn]][L[sp]]) { adj[sp][sn] = 0; continue; }
                if (comm[R[sn]][R[sp]]) { adj[sp][sn] = 0; continue; }

                adj[sp][sn] = 1;
            }

        init = true;
    }

    /* DP arrays over states of a pair of consecutive rows. */
    static seq_t dp[64];
    static seq_t dp_next[64];

    /* Base: arrays consisting of two rows (n == 1). */
    for (int i = 0; i < 64; ++i)
        dp[i] = (seq_t)(!comm[L[i]][R[i]]);   /* 1 if horizontal OK, else 0 */

    if (n == 1)
    {
        seq_t sum = 0;
        for (int i = 0; i < 64; ++i) sum += dp[i];
        return sum;
    }

    /* Transition for each additional row. */
    for (int step = 2; step <= (int)n; ++step)
    {
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        for (int sp = 0; sp < 64; ++sp)
        {
            if (dp[sp] == 0) continue;
            seq_t val = dp[sp];
            for (int sn = 0; sn < 64; ++sn)
                if (adj[sp][sn])
                    dp_next[sn] += val;
        }

        /* Prepare for next step. */
        for (int i = 0; i < 64; ++i) dp[i] = dp_next[i];
    }

    /* Sum over all final states. */
    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += dp[i];
    return result;
}
