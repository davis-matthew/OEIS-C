/*
 * A028469  Number of perfect matchings in graph P_{7} X P_{2n}.
 * https://oeis.org/A028469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a transfer‑matrix DP to count the perfect matchings of a 7×(2n) grid. */
#include <stdint.h>

/* Transition entry: next mask and number of ways to obtain it from a given current mask. */
typedef struct {
    uint8_t next;
    uint32_t cnt;
} TransEntry;

/* Transition tables. */
static TransEntry trans_list[128][128];
static int trans_len[128] = {0};
static uint32_t trans_counts[128][128];
static int trans_built = 0;

/* Recursive helper to enumerate all ways to fill a column given the
 * incoming mask 'mask' and the mask of cells reserved for the next column. */
static void dfs_mask(uint8_t mask, uint8_t next_mask, uint8_t orig_mask)
{
    /* Find the first free cell in the current column. */
    for (int r = 0; r < 7; ++r) {
        if (!(mask & (1u << r))) {
            /* Try a vertical domino if it fits. */
            if (r + 1 < 7 && !(mask & (1u << (r + 1)))) {
                dfs_mask(mask | (1u << r) | (1u << (r + 1)), next_mask, orig_mask);
            }
            /* Place a horizontal domino: occupy this cell and reserve the cell
             * in the next column. */
            dfs_mask(mask | (1u << r), next_mask | (1u << r), orig_mask);
            return;
        }
    }
    /* All cells of the current column are now filled. Record the resulting
     * mask for the next column. */
    trans_counts[orig_mask][next_mask] += 1u;
}

/* Build transition tables once. */
static void build_transitions(void)
{
    if (trans_built) return;

    /* Clear counters. */
    for (int i = 0; i < 128; ++i) {
        for (int j = 0; j < 128; ++j) {
            trans_counts[i][j] = 0u;
        }
    }

    /* Enumerate transitions for every possible incoming mask. */
    for (int m = 0; m < 128; ++m) {
        dfs_mask((uint8_t)m, (uint8_t)0, (uint8_t)m);
    }

    /* Pack non‑zero transitions into lists for quick traversal. */
    for (int m = 0; m < 128; ++m) {
        int pos = 0;
        for (int n = 0; n < 128; ++n) {
            if (trans_counts[m][n]) {
                trans_list[m][pos].next = (uint8_t)n;
                trans_list[m][pos].cnt  = trans_counts[m][n];
                ++pos;
            }
        }
        trans_len[m] = pos;
    }

    trans_built = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Prepare the transition tables. */
    build_transitions();

    /* Number of columns in the grid. */
    idx_t total_cols = n << 1;          /* 2 * n */
    if (total_cols == 0) return (seq_t)1; /* a(0) = 1 */

    /* DP arrays: dp[mask] holds the number of ways to reach the current
     * column with 'mask' pending for the next column. */
    seq_t dp[128];
    seq_t ndp[128];

    /* Initialise DP. */
    for (int i = 0; i < 128; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process every column. */
    for (idx_t col = 0; col < total_cols; ++col) {
        /* Reset next‑column DP array. */
        for (int i = 0; i < 128; ++i) ndp[i] = (seq_t)0;

        for (int cur = 0; cur < 128; ++cur) {
            seq_t cur_val = dp[cur];
            if (cur_val == (seq_t)0) continue;

            for (int t = 0; t < trans_len[cur]; ++t) {
                const TransEntry *e = &trans_list[cur][t];
                seq_t add = cur_val * (seq_t)e->cnt;
                ndp[e->next] += add;
            }
        }

        /* Copy results back into dp for the next iteration. */
        for (int i = 0; i < 128; ++i) dp[i] = ndp[i];
    }

    /* After all columns are processed, only the zero mask represents a
     * fully tiled board. */
    return dp[0];
}
