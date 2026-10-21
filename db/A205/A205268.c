/*
 * A205268  Number of (n+2)X5 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally, and new values 0..1 introduced in row major order.
 * https://oeis.org/A205268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A205268: Number of (n+2)×5 binary arrays with the
 * 3×3 subblock constraint, divided by 2 to account for
 * complement symmetry.
 * The counting is done by dynamic programming over rows.
 * See comment in source for details. */

#include <stdbool.h>
#include <stdint.h>

/* check if a 3×3 block (rows a,b,c) is valid.
 * a,b,c are 5‑bit masks (columns 0..4). */
static bool block_valid(int a, int b, int c)
{
    /* For each of the three possible horizontal lines. */
    for (int col = 0; col <= 2; ++col) {
        bool ok = false;

        /* horizontal line in row a */
        if ((((a >> col) & 7) == 0) || (((a >> col) & 7) == 7))
            ok = true;
        /* horizontal line in row b */
        if ((((b >> col) & 7) == 0) || (((b >> col) & 7) == 7))
            ok = true;
        /* horizontal line in row c */
        if ((((c >> col) & 7) == 0) || ((((c >> col) & 7) == 7)))
            ok = true;

        /* vertical lines: check all three columns inside the block */
        for (int d = 0; d <= 2; ++d) {
            int p = (a >> (col + d)) & 1;
            int q = (b >> (col + d)) & 1;
            if (p != q) continue;
            int r = (c >> (col + d)) & 1;
            if (p == r) {
                ok = true;
                break;
            }
        }

        /* NW‑SE diagonal inside the block */
        if (!ok) {
            int p = (a >> col) & 1;
            int q = (b >> (col + 1)) & 1;
            if (p == q) {
                int r = (c >> (col + 2)) & 1;
                if (p == r)
                    ok = true;
            }
        }

        if (!ok)
            return false;
    }
    return true;
}

/* transition masks: for each pair (p2,p1) store all cur
 * rows that keep all 3×3 windows valid.  Index is
 * (p2 << 5) | p1, 0 <= p2,p1 < 32. */
static uint32_t transition_masks[1024];
static bool transition_computed = false;

static void compute_transitions(void)
{
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p1 = 0; p1 < 32; ++p1) {
            uint32_t mask = 0;
            for (int cur = 0; cur < 32; ++cur) {
                if (block_valid(p2, p1, cur))
                    mask |= (1U << cur);
            }
            transition_masks[(p2 << 5) | p1] = mask;
        }
    }
}

seq_t a(idx_t n)
{
    if (!transition_computed) {
        compute_transitions();
        transition_computed = true;
    }

    /* DP: dp[state] = number of ways to reach the state after
     * processing the current row.  State is encoded as
     * (prev2 << 5) | prev1. */
    static seq_t dp[1024];
    static seq_t next[1024];

    /* Initialise with all possible first two rows. */
    for (int i = 0; i < 1024; ++i) dp[i] = 0;
    for (int p2 = 0; p2 < 32; ++p2)
        for (int p1 = 0; p1 < 32; ++p1)
            dp[(p2 << 5) | p1] = 1;

    /* Append n additional rows; total rows = 2 + n = n+2. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 1024; ++i)
            next[i] = 0;
        for (int state = 0; state < 1024; ++state) {
            seq_t cnt = dp[state];
            if (!cnt) continue;
            int prev2 = state >> 5;
            int prev1 = state & 0x1F;
            uint32_t mask = transition_masks[(prev2 << 5) | prev1];
            while (mask) {
                int cur = __builtin_ctz(mask);
                int dest = (prev1 << 5) | cur;
                next[dest] += cnt;
                mask &= mask - 1;
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 1024; ++i)
            dp[i] = next[i];
    }

    /* Sum over all final states.  This gives twice the OEIS value
     * because every valid array has a complement that is also counted.
     * Divide by two. */
    seq_t res = 0;
    for (int i = 0; i < 1024; ++i)
        res += dp[i];
    return res >> 1;
}
