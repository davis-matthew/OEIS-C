/*
 * A220055  Number of tilings of a 5 X n rectangle using right trominoes and 1
 *          X 1 tiles.
 * https://oeis.org/A220055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Dynamic programming over columns.
 *  a(0)=1, a(1)=1.  Each column is represented by a 5‑bit mask of cells already
 *  occupied by a tile that reached it from the previous column.  Transitions
 *  are pre‑computed: from a current mask to any next mask, all ways to fill
 *  the current column using monominoes and right trominoes (all 4 rotations).
 *  The counting is exact for any seq_t implementation.  The transition
 *  table is built on first use.  Complexity: O(n·32·k) where k is the
 *  number of transitions from a mask (at most 32).  Works for n up to
 *  10000 in practice. */

#include <stddef.h>
#include <stdint.h>

/* Transition entry: next mask and number of ways to reach it. */
typedef struct {
    uint8_t next;   /* 5‑bit mask for the next column */
    seq_t   cnt;    /* number of tilings producing this next mask */
} trans_t;

static trans_t trans[32][32];
static size_t  trans_count[32];
static int     trans_ready = 0;   /* set to 1 after table is built */

/* Depth‑first search that counts all ways to fill a single column.
 * Parameters:
 *   cur_mask    – which cells in the current column are already filled.
 *   next_mask   – cells that are filled in the next column by tiles
 *                 protruding from the current column.
 *   cnt[next]   – array accumulating the number of ways for each next mask.
 */
static void dfs(uint8_t cur_mask, uint8_t next_mask, seq_t *cnt)
{
    /* Find first free cell in the current column. */
    int r;
    for (r = 0; r < 5; ++r)
        if (!(cur_mask & (1 << r)))
            break;

    if (r == 5) {      /* column completely filled */
        cnt[next_mask] += (seq_t)1;
        return;
    }

    /* Place a monomino at (current column, row r). */
    dfs(cur_mask | (1 << r), next_mask, cnt);

    /* Place a tromino with a vertical pair in the current column. */
    if (r < 4 && !(cur_mask & (1 << (r + 1)))) {
        /* Orientation A: protrudes into next column at row r. */
        if (!(next_mask & (1 << r)))
            dfs(cur_mask | (1 << r) | (1 << (r + 1)),
                next_mask | (1 << r), cnt);

        /* Orientation B: protrudes into next column at row r+1. */
        if (!(next_mask & (1 << (r + 1))))
            dfs(cur_mask | (1 << r) | (1 << (r + 1)),
                next_mask | (1 << (r + 1)), cnt);
    }

    /* Place a tromino whose vertical pair lies in the next column. */
    /* (Only the cell in the current column is filled.) */
    if (r < 4) {
        /* Orientation C: next column cells r and r+1. */
        if (!(next_mask & (1 << r)) && !(next_mask & (1 << (r + 1))))
            dfs(cur_mask | (1 << r),
                next_mask | (1 << r) | (1 << (r + 1)), cnt);
    }
    if (r > 0) {
        /* Orientation D: next column cells r-1 and r. */
        if (!(next_mask & (1 << (r - 1))) && !(next_mask & (1 << r)))
            dfs(cur_mask | (1 << r),
                next_mask | (1 << (r - 1)) | (1 << r), cnt);
    }
}

/* Build the transition table once. */
static void generate_transitions(void)
{
    for (int mask = 0; mask < 32; ++mask) {
        seq_t local_cnt[32] = {0};
        dfs((uint8_t)mask, 0, local_cnt);
        size_t used = 0;
        for (int next = 0; next < 32; ++next) {
            if (local_cnt[next] != (seq_t)0) {
                trans[mask][used].next = (uint8_t)next;
                trans[mask][used].cnt  = local_cnt[next];
                ++used;
            }
        }
        trans_count[mask] = used;
    }
    trans_ready = 1;
}

/* Compute a(n) for sequence A220055. */
seq_t a(idx_t n)
{
    if (!trans_ready)
        generate_transitions();

    seq_t dp[32]     = {0};
    seq_t dp_next[32] = {0};
    dp[0] = (seq_t)1;                     /* start with empty rectangle */

    for (idx_t col = 0; col < n; ++col) {
        /* clear next DP array */
        for (size_t i = 0; i < 32; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate transitions from each possible previous mask */
        for (size_t mask = 0; mask < 32; ++mask) {
            if (dp[mask] == (seq_t)0)
                continue;
            size_t tcnt = trans_count[mask];
            for (size_t t = 0; t < tcnt; ++t) {
                uint8_t nxt = trans[mask][t].next;
                seq_t add   = dp[mask] * trans[mask][t].cnt;
                dp_next[nxt] += add;
            }
        }

        /* copy next into current for next iteration */
        for (size_t i = 0; i < 32; ++i)
            dp[i] = dp_next[i];
    }

    return dp[0];  /* all protruding cells must be 0 at the end */
}
