/*
 * A236945  Number of (n+1)X(4+1) 0..3 arrays with the maximum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with values 0–3 such that *
 *            every 2×2 subblock has the same maximum. For each possible
 *            maximum * m = 0..3 we count all arrays whose maximum is exactly
 *            m, then sum the * four contributions. The algorithm uses dynamic
 *            programming over the * rows: each row is represen...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Compatibility table for two masks.  true if every 2×2 block
 * formed by the two masks contains at least one '1' bit. */
static bool compat[32][32];
static bool compat_inited = false;

static void init_compat(void)
{
    if (compat_inited) return;
    compat_inited = true;
    for (int s = 0; s < 32; ++s) {
        for (int t = 0; t < 32; ++t) {
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                /* Check the four cells of the 2×2 block. */
                if (!(((s >> j) & 1) || ((s >> (j + 1)) & 1) ||
                     ((t >> j) & 1) || ((t >> (j + 1)) & 1))) {
                    ok = false;
                    break;
                }
            }
            compat[s][t] = ok;
        }
    }
}

/* Small integer power for seq_t (exp ≤ 5). */
static seq_t power_seq_t(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Count arrays for a fixed maximum value m (0…3).  */
static seq_t count_for_m(idx_t n, int m)
{
    init_compat();
    idx_t rows = n + 1;          /* number of rows in the array */
    seq_t weight[32];

    /* Weight per mask: number of ways to assign non‑m entries in a row. */
    for (int mask = 0; mask < 32; ++mask) {
        int pc = __builtin_popcount((unsigned)mask);
        if (m == 0) {
            weight[mask] = (pc == 5) ? 1 : 0;   /* all entries must be 0 */
        } else {
            weight[mask] = power_seq_t((seq_t)m, 5 - pc);
        }
    }

    seq_t dp[32], next[32];
    for (int i = 0; i < 32; ++i) dp[i] = weight[i];

    for (idx_t step = 1; step < rows; ++step) {
        for (int t = 0; t < 32; ++t) next[t] = 0;

        for (int s = 0; s < 32; ++s) {
            if (dp[s] == 0) continue;
            seq_t val = dp[s];
            for (int t = 0; t < 32; ++t) {
                if (compat[s][t]) next[t] += val * weight[t];
            }
        }

        /* Copy next into dp – use elementwise assignment to avoid
         * copying objects with internal pointers (prevents double free). */
        for (int i = 0; i < 32; ++i) dp[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 32; ++i) total += dp[i];
    return total;
}

/* External function required by the build.  */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (int m = 0; m <= 3; ++m) result += count_for_m(n, m);
    return result;
}
