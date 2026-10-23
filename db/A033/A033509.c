/*
 * A033509  Number of matchings in graph P_{6} X P_{n}.
 * https://oeis.org/A033509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of matchings in the graph P6 × Pn. The values are
 *            computed by a transfer‑matrix dynamic programming that keeps,
 *            for each column, the bitmask of vertices that are matched to the
 *            next column. The DP uses 2^6 = 64 states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* number of rows (fixed to 6) */
#define R 6
/* total number of states */
#define S 64

/* transition matrix: trans[s][t] = number of ways to go from state s
   to state t in one column. */
static seq_t trans[S][S];
static bool trans_built = false;

/* Recursive helper to build the transition matrix for one source state.
   occ:  bitmask of vertices already occupied in this column
   t:    bitmask of vertices that will be matched to the right (pending)
   s:    original source state (used to index trans). */
static void rec_trans(int pos, uint8_t occ, uint8_t t, uint8_t s)
{
    if (pos == R) {
        /* All vertices processed: record the transition. */
        trans[s][t] += (seq_t)1;
        return;
    }

    /* If this vertex is already matched (from the left), skip it. */
    if (occ & (1u << pos)) {
        rec_trans(pos + 1, occ, t, s);
    } else {
        /* Option 1: leave vertex unmatched. */
        rec_trans(pos + 1, occ, t, s);

        /* Option 2: match with the vertical neighbour if available. */
        if (pos + 1 < R && !(occ & (1u << (pos + 1)))) {
            rec_trans(pos + 1,
                      occ | (1u << pos) | (1u << (pos + 1)),
                      t,
                      s);
        }

        /* Option 3: match horizontally to the right. */
        rec_trans(pos + 1,
                  occ | (1u << pos),
                  t | (1u << pos),
                  s);
    }
}

/* Build the transition matrix once. */
static void build_trans(void)
{
    if (trans_built) return;

    for (uint8_t s = 0; s < S; ++s) {
        rec_trans(0, s, 0, s);
    }
    trans_built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* empty graph has one matching */

    build_trans();

    static seq_t dp[2][S];

    /* initialise DP for column 0 */
    for (int i = 0; i < S; ++i) dp[0][i] = 0;
    dp[0][0] = (seq_t)1;

    int cur = 0, nxt = 1;
    for (idx_t col = 0; col < n; ++col) {
        for (int i = 0; i < S; ++i) dp[nxt][i] = 0;
        for (int s = 0; s < S; ++s) {
            seq_t val = dp[cur][s];
            if (val == 0) continue;
            for (int t = 0; t < S; ++t) {
                seq_t ways = trans[s][t];
                if (ways == 0) continue;
                dp[nxt][t] += val * ways;
            }
        }
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }
    return dp[cur][0];
}
