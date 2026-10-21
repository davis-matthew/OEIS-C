/*
 * A219076  Number of n X 6 binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Implements DP counting of binary n×6 arrays where every 0 has a
 *            horizontal or diagonal neighbor that is 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Sequence function a(idx_t n) – defined with external linkage */
/*  seq_t and idx_t are provided by the build system.             */
/* ------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t diag_mask[64];                /* (r<<1)|(r>>1) & 0x3F */
static uint8_t pend_mask[64][64];           /* zero bits unresolved for r */
static bool tables_inited = false;

static void init_tables(void)
{
    if (tables_inited) return;
    for (int r = 0; r < 64; ++r) {
        diag_mask[r] = ((r << 1) | (r >> 1)) & 0x3F;
    }
    for (int p = 0; p < 64; ++p) {
        uint8_t p_diag = ((p << 1) | (p >> 1)) & 0x3F;
        for (int r = 0; r < 64; ++r) {
            uint8_t r_hor  = diag_mask[r];
            uint8_t zeros  = (~r) & 0x3F;
            uint8_t uncovered = zeros & ~(r_hor | p_diag);
            pend_mask[p][r] = uncovered;
        }
    }
    tables_inited = true;
}

typedef struct {
    uint8_t pat;    /* previous row pattern 0..63 */
    uint8_t pend;   /* pending zeros mask (0..63) */
} State;

seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays: dp[pat][pend] gives count of ways to reach state */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];
    /* Active state lists */
    static State active[800];
    static State next_active[800];
    int active_sz = 0, next_sz = 0;

    /* initialize dp to zero */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;

    /* ----- Initial row (row 1) ----- */
    for (int r = 0; r < 64; ++r) {
        uint8_t hor = diag_mask[r];
        uint8_t zeros = (~r) & 0x3F;
        uint8_t pend = zeros & ~hor;           /* zeros lacking horizontal 1 */
        dp[r][pend] += 1;
        active[active_sz].pat = (uint8_t)r;
        active[active_sz].pend = pend;
        ++active_sz;
    }

    if (n == 1) {                       /* offset 1: first term */
        seq_t ans = 0;
        for (int i = 0; i < active_sz; ++i)
            if (active[i].pend == 0)
                ans += dp[active[i].pat][0];
        return ans;
    }

    /* ----- Process rows 2 .. n ----- */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = 0;

        next_sz = 0;

        for (int s = 0; s < active_sz; ++s) {
            uint8_t prev_pat = active[s].pat;
            uint8_t prev_pend = active[s].pend;
            seq_t val = dp[prev_pat][prev_pend];

            for (int r = 0; r < 64; ++r) {
                /* Check if pending zeros from prev row are resolved by
                   diagonal neighbors of current row `r`. */
                if (prev_pend & ~diag_mask[r]) continue;

                uint8_t next_pend = pend_mask[prev_pat][r];

                if (ndp[r][next_pend] == 0) {
                    /* first time we hit this state: store for next loop */
                    next_active[next_sz].pat = (uint8_t)r;
                    next_active[next_sz].pend = next_pend;
                    ++next_sz;
                }
                ndp[r][next_pend] += val;
            }
        }

        /* move ndp to dp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];

        /* update active list for next iteration */
        for (int i = 0; i < next_sz; ++i) {
            active[i] = next_active[i];
        }
        active_sz = next_sz;
    }

    /* ----- Final answer: all states with no unresolved zeros ----- */
    seq_t ans = 0;
    for (int i = 0; i < active_sz; ++i)
        if (active[i].pend == 0)
            ans += dp[active[i].pat][0];

    return ans;
}
