/*
 * A196132  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,2,1,3,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via a transfer‑matrix recurrence. The matrix is
 *            derived from the local constraint that each entry x ∈
 *            {0,1,2,3,4} must equal the number of its horizontal and vertical
 *            neighbours with value f(x) = {0,2,1,3,4} for x = 0,1,2,3,4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The mapping from a value to the neighbour value that is counted. */
static const int f[5] = {0, 2, 1, 3, 4};

/* Total number of possible rows (2 columns of values 0..4), plus one dummy state.
   Dummy state has values -1,-1 and represents the absence of a real row
   (used for the top boundary). */
#define STATE_COUNT 26          /* 25 real states + 1 dummy */
#define REAL_STATES 25          /* indices 0 .. 24 are real */
#define DUMMY_STATE 25          /* dummy state index */

/* Precomputed data for each state: the two cell values and the left/right
   contribution for each cell.  */
static int row_val[STATE_COUNT][2];
static int lr_contrib[STATE_COUNT][2];

/* For every triple (up, cur, down) we store whether the middle row
   satisfies its constraints given the up and down neighbours. */
static int valid_triple[STATE_COUNT][STATE_COUNT][STATE_COUNT];

/* For each pair (prev, cur) we store the list of next rows that form
   a valid triple (prev, cur, next).  The neighbours are condensed
   into a single contiguous integer buffer.  */
static int *next_rows[STATE_COUNT * STATE_COUNT];
static int  next_cnt [STATE_COUNT * STATE_COUNT];
static int  adj_storage[STATE_COUNT * STATE_COUNT * STATE_COUNT];

/* ------------------------------------------------------------------------ */
/* Helper: ensures that double loops over states are only performed once. */
static void init_static(void) {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    int i, j, k, idx;
    /* Build real states. */
    int s = 0;
    for (i = 0; i <= 4; ++i)
        for (j = 0; j <= 4; ++j) {
            row_val[s][0] = i;
            row_val[s][1] = j;
            lr_contrib[s][0] = (row_val[s][1] == f[row_val[s][0]]) ? 1 : 0;
            lr_contrib[s][1] = (row_val[s][0] == f[row_val[s][1]]) ? 1 : 0;
            ++s;
        }

    /* Dummy state (index 25).  Member values are -1, so comparisons
       with any valid f(v) will fail. */
    row_val[DUMMY_STATE][0] = -1;
    row_val[DUMMY_STATE][1] = -1;
    lr_contrib[DUMMY_STATE][0] = 0;
    lr_contrib[DUMMY_STATE][1] = 0;

    /* Precompute valid triples. */
    for (i = 0; i < STATE_COUNT; ++i) {
        for (j = 0; j < STATE_COUNT; ++j) {
            for (k = 0; k < STATE_COUNT; ++k) {
                /* `i` is the up row, `j` is the current row, `k` is the down row. */
                int ok = 1;
                for (int col = 0; col < 2; ++col) {
                    int v = row_val[j][col];
                    if (v < 0 || v > 4) { ok = 0; break; }

                    int other = 1 - col;
                    int lr = lr_contrib[j][col];
                    int up = (row_val[i][col] == f[v]) ? 1 : 0;
                    int need_down = v - lr - up;
                    if (need_down < 0 || need_down > 1) { ok = 0; break; }

                    if (need_down == 1) {
                        if (row_val[k][col] != f[v]) { ok = 0; break; }
                    } else { /* need_down == 0 */
                        if (row_val[k][col] == f[v]) { ok = 0; break; }
                    }
                }
                valid_triple[i][j][k] = ok;
            }
        }
    }

    /* Build adjacency list for quick transition. */
    int *buf = adj_storage;
    for (i = 0; i < REAL_STATES; ++i) {          /* prev row */
        for (j = 0; j < REAL_STATES; ++j) {      /* cur row */
            idx = i * STATE_COUNT + j;
            next_rows[idx] = buf;
            int cnt = 0;
            for (k = 0; k < REAL_STATES; ++k) {  /* down row */
                if (valid_triple[i][j][k]) {
                    *buf++ = k;
                    ++cnt;
                }
            }
            next_cnt[idx] = cnt;
        }
    }
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_static();          /* ensure tables have been built once */

    if (n == 0) return 0;   /* out of range; sequence is defined for n>=1 */

    /* --------------------------------------------------------------------
       Special treatment for n == 1: only left/right neighbours exist. */
    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < REAL_STATES; ++s) {
            int ok = 1;
            for (int col = 0; col < 2; ++col) {
                int v = row_val[s][col];
                int lr = lr_contrib[s][col];
                if (lr != v) { ok = 0; break; }
            }
            if (ok) total = total + 1;   /* use (+) which works for all seq_t flavours */
        }
        return total;
    }

    /* --------------------------------------------------------------------
       DP: dp[prev][cur] counts of arrays up to current row where
       the last two rows are `prev` and `cur`,
       and all rows except the last one are fully satisfied. */
    seq_t dp[STATE_COUNT * STATE_COUNT];
    seq_t newdp[STATE_COUNT * STATE_COUNT];
    /* initialize to zero */
    for (int i = 0; i < STATE_COUNT * STATE_COUNT; ++i) {
        dp[i] = 0;
        newdp[i] = 0;
    }

    /* Initialise for the first two rows.  The up neighbor of row 1 is the dummy. */
    for (int a = 0; a < REAL_STATES; ++a) {
        for (int b = 0; b < REAL_STATES; ++b) {
            if (valid_triple[DUMMY_STATE][a][b]) {
                dp[a * STATE_COUNT + b] = 1;
            }
        }
    }

    /* Process further rows (3 .. n).  For each transition we check that
       the middle row `prev`-`cur` pair satisfies constraints with the new
       down neighbour. */
    for (int row = 3; row <= n; ++row) {
        /* Zero newdp */
        for (int i = 0; i < STATE_COUNT * STATE_COUNT; ++i) newdp[i] = 0;

        for (int prev = 0; prev < REAL_STATES; ++prev) {
            for (int cur = 0; cur < REAL_STATES; ++cur) {
                seq_t ways = dp[prev * STATE_COUNT + cur];
                if (ways == 0) continue;

                int idx = prev * STATE_COUNT + cur;
                for (int id = 0; id < next_cnt[idx]; ++id) {
                    int next = next_rows[idx][id];
                    newdp[cur * STATE_COUNT + next] = newdp[cur * STATE_COUNT + next] + ways;
                }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < STATE_COUNT * STATE_COUNT; ++i) dp[i] = newdp[i];
    }

    /* After the last row, the last row (`cur`) hasn't been checked for
       its down neighbour.  We now add the count of all situations where
       the last row `cur` satisfies its constraints with the previous
       row `prev` and with no down neighbour. */
    seq_t total = 0;
    for (int prev = 0; prev < REAL_STATES; ++prev) {
        for (int cur = 0; cur < REAL_STATES; ++cur) {
            seq_t ways = dp[prev * STATE_COUNT + cur];
            if (ways == 0) continue;

            /* Check that cur satisfies the constraints with up=prev and down=none */
            int ok = 1;
            for (int col = 0; col < 2; ++col) {
                int v = row_val[cur][col];
                int lr = lr_contrib[cur][col];
                int up = (row_val[prev][col] == f[v]) ? 1 : 0;
                if (lr + up != v) { ok = 0; break; }
            }
            if (ok) total = total + ways;
        }
    }
    return total;
}
