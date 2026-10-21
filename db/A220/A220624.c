/*
 * A220624  Number of ways to reciprocally link elements of an 5Xn array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A220624: Number of ways to reciprocally link elements of a 5×n array
   either to themselves or to exactly one horizontal, diagonal, or antidiagonal
   neighbor.  The sequence is computed by a transfer‑matrix dynamic program
   over the columns of the array.  For each column we keep a 5‑bit mask of
   vertices that have already been matched to the left neighbour.  For every
   mask we pre‑compute a 32×32 transition matrix that counts how many ways
   the remaining vertices of the current column can be matched to the next
   column.  The DP runs in O(n·32²) time and uses only integer arithmetic,
   so it works for both fixed‑size integer builds and arbitrary‑precision
   builds (whatever the seq_t type is).
*/
#include <stdint.h>
#include <stdbool.h>

static seq_t M[32][32];
static bool init_once = false;

/* adjacency: true if a cell in current column can match
 * to a cell in next column when rows differ by ≤1
 */
static const bool adj[5][5] = {
    {true,true,false,false,false},
    {true,true,true,false,false},
    {false,true,true,true,false},
    {false,false,true,true,true},
    {false,false,false,true,true}
};

/* recursively enumerate matchings from the set of unmatched rows of
 * the current column (u_mask) to vertices in the next column.
 * matched_mask accumulates which rows of the next column are matched.
 * counts[matched_mask] is incremented for each distinct matching.
 */
static void dfs_match(uint32_t u_mask, uint32_t matched_mask, seq_t *counts)
{
    if (u_mask == 0u) {
        counts[matched_mask] += (seq_t)1;
        return;
    }
    /* find the first unmatched row in the current column */
    int u = 0;
    while (!(u_mask & (1u << u))) u++;
    uint32_t rest = u_mask & ~(1u << u);

    /* Option 1: the current row self‑loops (unmatched). */
    dfs_match(rest, matched_mask, counts);

    /* Option 2: match the current row with any compatible row
     * in the next column that is still free.
     */
    for (int v = 0; v < 5; ++v) {
        if (!(matched_mask & (1u << v)) && adj[u][v]) {
            dfs_match(rest, matched_mask | (1u << v), counts);
        }
    }
}

/* initialize the transition matrix M */
static void init_transition(void)
{
    for (int m = 0; m < 32; ++m) {
        uint32_t u_mask = (~(uint32_t)m) & 0x1Fu;  /* rows not matched from left */
        seq_t counts[32] = {0};
        dfs_match(u_mask, 0u, counts);
        for (int k = 0; k < 32; ++k) {
            M[m][k] = counts[k];
        }
    }
    init_once = true;
}

seq_t a(idx_t n)
{
    if (!init_once) init_transition();

    seq_t dp[32] = {0}, next[32] = {0};
    dp[0] = (seq_t)1;              /* no vertices are matched from left before first column */

    for (idx_t col = (idx_t)0; col < n; ++col) {
        for (int i = 0; i < 32; ++i) next[i] = (seq_t)0;
        for (int m = 0; m < 32; ++m) {
            if (dp[m] == (seq_t)0) continue;
            seq_t val = dp[m];
            for (int k = 0; k < 32; ++k) {
                if (M[m][k] != (seq_t)0) {
                    next[k] += val * M[m][k];
                }
            }
        }
        for (int i = 0; i < 32; ++i) dp[i] = next[i];
    }
    return dp[0];   /* after processing all columns, no vertex may be matched to the right */
}
