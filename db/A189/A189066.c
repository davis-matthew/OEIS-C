/*
 * A189066  Number of 4Xn binary arrays without the pattern 0 1 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A189066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays without the pattern 0 1 0
 *            antidiagonally or horizontally. The algorithm uses a DP that
 *            keeps the last two columns as a state. A pre‑computed transition
 *            table tells whether a new column can follow the previous two
 *            when the forbidden patterns are considered. For a...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Transition table: true if columns a,b,c are allowed together
   (i.e., no forbidden pattern involving them).  The state is encoded
   as (a,b) -> 16*a + b. */
static bool trans_init = false;
static unsigned char trans[256][16];

static bool is_allowed(uint16_t a, uint16_t b, uint16_t c)
{
    /* horizontal 010 in any row */
    for (int r = 0; r < 4; ++r) {
        if (((a >> r) & 1) == 0 &&
            ((b >> r) & 1) == 1 &&
            ((c >> r) & 1) == 0)
            return false;
    }
    /* antidiagonal 010 in rows (0,1,2) or (1,2,3) */
    for (int r = 0; r < 2; ++r) {
        if (((a >> r)     & 1) == 0 &&
            ((b >> (r+1)) & 1) == 1 &&
            ((c >> (r+2)) & 1) == 0)
            return false;
    }
    return true;
}

static void init_trans(void)
{
    if (trans_init) return;
    for (uint16_t a = 0; a < 16; ++a)
        for (uint16_t b = 0; b < 16; ++b) {
            uint16_t pair = (a << 4) | b;
            for (uint16_t c = 0; c < 16; ++c)
                trans[pair][c] = is_allowed(a, b, c) ? 1 : 0;
        }
    trans_init = true;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;

    init_trans();

    /* DP arrays for counts of states (last two columns). */
    seq_t dp[256];
    seq_t nextdp[256];

    /* Initialise all 256 states with one way (two arbitrary columns). */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)1;

    /* For columns 2 .. n-1 (zero‑based). */
    for (idx_t col = 2; col < n; ++col) {
        /* Reset nextdp. */
        for (int i = 0; i < 256; ++i) nextdp[i] = (seq_t)0;

        for (int idx = 0; idx < 256; ++idx) {
            seq_t ways = dp[idx];
            if (ways == (seq_t)0) continue;

            int a = idx >> 4;          /* previous column */
            int b = idx & 15;          /* current column */

            for (int nxt = 0; nxt < 16; ++nxt) {
                if (!trans[idx][nxt]) continue;
                int newidx = (b << 4) | nxt;
                nextdp[newidx] += ways;
            }
        }

        /* copy nextdp to dp for next iteration */
        for (int i = 0; i < 256; ++i) dp[i] = nextdp[i];
    }

    /* Sum over all final states. */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 256; ++i) sum += dp[i];
    return sum;
}
