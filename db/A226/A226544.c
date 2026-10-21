/*
 * A226544  Number of tilings of a 10 X n rectangle using 1 X 1 squares and
 *          L-tiles.
 * https://oeis.org/A226544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A226544: number of tilings of a 10×n rectangle
 * with 1×1 squares and L‑tiles (2×2 square with upper right
 * cell missing, no rotations).  The DP keeps a 10‑bit mask of
 * cells waiting to be filled in the next row.  For each mask
 * all possible completions of the current row are precomputed;
 * then the DP is run for n rows.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define TRANS_MAX 2048       /* enough to store transitions for every mask */

typedef struct {
    uint16_t next;   /* mask of cells occupied in the next row            */
    uint16_t count;  /* how many ways lead to this next mask from start   */
} Trans;

/* transitions for every start mask */
static Trans trans_table[1024][TRANS_MAX];
static size_t trans_sz[1024];

/* recursive helper to generate transitions for a given start mask */
static void dfs(uint16_t filled, uint16_t next_mask, int pos, uint16_t start_mask) {
    while (pos < 10 && (filled & (1u << pos))) pos++;
    if (pos == 10) {
        /* finished the current row: record the transition */
        size_t i;
        for (i = 0; i < trans_sz[start_mask]; ++i) {
            if (trans_table[start_mask][i].next == next_mask) {
                trans_table[start_mask][i].count++;
                return;
            }
        }
        /* new transition entry */
        trans_table[start_mask][trans_sz[start_mask]].next  = next_mask;
        trans_table[start_mask][trans_sz[start_mask]].count = 1;
        trans_sz[start_mask]++;
        return;
    }

    /* place a 1×1 square at pos */
    dfs(filled | (1u << pos), next_mask, pos + 1, start_mask);

    /* place an L‑tile covering pos and pos+1 in current row,
       and occupying pos in the next row */
    if (pos + 1 < 10 && !(filled & (1u << (pos + 1)))) {
        dfs(filled | (1u << pos) | (1u << (pos + 1)),
            next_mask | (1u << pos),
            pos + 2,
            start_mask);
    }
}

/* build all transitions once */
static void build_transitions(void) {
    for (int m = 0; m < 1024; ++m) {
        trans_sz[m] = 0;
        dfs((uint16_t)m, 0, 0, (uint16_t)m);
    }
}

static seq_t dp[2][1024];

/* sequence function */
seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)1;

    static int initialized = 0;
    if (!initialized) {
        build_transitions();
        initialized = 1;
    }

    /* initialize DP for row 0 */
    for (int i = 0; i < 1024; ++i) dp[0][i] = (seq_t)0;
    dp[0][0] = (seq_t)1;

    int cur = 0, nxt = 1;
    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < 1024; ++i) dp[nxt][i] = (seq_t)0;
        for (int mask = 0; mask < 1024; ++mask) {
            seq_t ways = dp[cur][mask];
            if (ways == (seq_t)0) continue;
            for (size_t t = 0; t < trans_sz[mask]; ++t) {
                uint16_t next_mask = trans_table[mask][t].next;
                uint16_t cnt       = trans_table[mask][t].count;
                seq_t add = ways * (seq_t)cnt;
                dp[nxt][next_mask] += add;
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    return dp[cur][0];
}
