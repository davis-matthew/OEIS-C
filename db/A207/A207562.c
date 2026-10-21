/*
 * A207562  Number of nX6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays avoiding horizontal patterns
 *            001 and 010 and vertical patterns 011 and 110. Computed with a
 *            DP over the 21 valid 6‑bit rows and 441 pair states
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const int ROWS_BITS = 6;
static const int FULL_MASK = (1 << ROWS_BITS) - 1;

static int valid_rows[21];
static int row_to_idx[64];
static int pair_to_idx[21][21];        /* pair (i,j) -> linear idx 0..440 */
static int pair_from_idx[441];         /* reverse mapping of above */
static int trans_to[441][21];          /* adjacency list, max 21 dests */
static int trans_cnt[441];

/* Initialize all static tables only once */
static void init_tables(void)
{
    static bool init = false;
    if (init) return;
    init = true;

    /* --- enumerate valid rows (avoid 001 and 010) --- */
    for (int r = 0; r < 64; ++r) {
        bool ok = true;
        for (int p = 0; p <= ROWS_BITS - 3; ++p) {
            int tri = (r >> p) & 7;
            if (tri == 1 || tri == 2) { /* 001 or 010 */
                ok = false;
                break;
            }
        }
        if (ok) {
            int idx = (int)(sizeof(valid_rows)/sizeof(int)) - 21; /* 0..20 */
            /* Find next free slot */
            for (int t = 0; t < 21; ++t) {
                if (t >= 21 ? false : true) {}
            }
        }
    }
    /* Above loop is inefficient; rewrite using indexing */
    int cnt = 0;
    for (int r = 0; r < 64; ++r) {
        bool ok = true;
        for (int p = 0; p <= ROWS_BITS - 3; ++p) {
            int tri = (r >> p) & 7;
            if (tri == 1 || tri == 2) { /* 001 or 010 */
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_rows[cnt++] = r;
            row_to_idx[r] = cnt - 1;
        }
    }
    /* Fill -1 for invalid rows */
    for (int r = 0; r < 64; ++r)
        if (row_to_idx[r] == 0) row_to_idx[r] = -1;
    /* --- mapping pairs to indices --- */
    cnt = 0;
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            pair_to_idx[i][j] = cnt;
            pair_from_idx[cnt++] = i * 21 + j;
        }
    }
    /* --- precompute transitions --- */
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            int a = valid_rows[i];
            int b = valid_rows[j];
            int mask1 = a & b;                     /* forced 1s */
            int mask0 = (~a) & b & FULL_MASK;      /* forced 0s (where a=0,b=1) */
            int src = pair_to_idx[i][j];
            int cidx = 0;
            for (int k = 0; k < 21; ++k) {
                int c = valid_rows[k];
                if ( (c & mask1) == mask1 && (c & mask0) == 0 ) {
                    int dst = pair_to_idx[j][k];   /* new pair (b,c) */
                    trans_to[src][cidx++] = dst;
                }
            }
            trans_cnt[src] = cidx;
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;            /* undefined, return 0 */

    /* number of valid rows */
    const seq_t m = (seq_t)21;

    if (n == 1) return m;

    if (n == 2) return m * m;

    /* DP arrays for 441 states */
    static seq_t dp[441];
    static seq_t ndp[441];
    for (int s = 0; s < 441; ++s) dp[s] = 0;

    /* initial: any pair of valid rows */
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            int idx = pair_to_idx[i][j];
            dp[idx] = 1;
        }
    }

    /* iterate rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int s = 0; s < 441; ++s) ndp[s] = 0;
        for (int s = 0; s < 441; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            int cnt = trans_cnt[s];
            for (int t = 0; t < cnt; ++t) {
                int dest = trans_to[s][t];
                ndp[dest] += cur;
            }
        }
        /* copy ndp to dp */
        for (int s = 0; s < 441; ++s) dp[s] = ndp[s];
    }

    /* sum all states to get total sequences */
    seq_t total = 0;
    for (int s = 0; s < 441; ++s) total += dp[s];
    return total;
}
