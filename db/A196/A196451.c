/*
 * A196451  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,2,4,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A196451: Number of n×3 arrays over {0,…,4} satisfying the
   neighbor‑count condition.
   This implementation uses a dynamic‑programming approach based on
   row‑transition rules.  The state space consists of pairs of consecutive
   rows; the transition from (row_{i-2},row_{i-1}) to (row_{i-1},row_i)
   is only possible when the middle row satisfies its constraints with
   the given above and below rows.  For n ≥ 3 the DP runs in
   O(n · 152 000) time, fast enough for n ≈ 10 000. */

#include <stddef.h>
#include <stdbool.h>

#define MAX_ROWS 125U         /* 5³ possible rows */
#define MAX_STATES (MAX_ROWS * MAX_ROWS)

extern seq_t a(idx_t n);  /* declaration only – implementation follows */

/* --------------------------------------------------------------------- */
/*  Utilities                                                    */
/* --------------------------------------------------------------------- */

/* Mapping for the required neighbor value for each cell value */
static const int MAP[5] = {3, 0, 2, 4, 1};

/* row_digits[row][col] – integer representation of a row (0..124) */
static int row_digits[MAX_ROWS][3];

/* Transition tables:
   trans_len[idx]  – number of successors of state idx
   trans[idx][k]   – successor state (cur*125 + nxt) indices
   The total table size is MAX_STATES × 64 integers (~4 MB). */
static unsigned char trans_len[MAX_STATES];
static int trans[MAX_STATES][64];

/* Flag indicating whether the tables have been initialized. */
static bool tables_built = false;

/* Return true if a given row satisfies its constraints with the
   supplied adjacent rows (above and/or below).  If an adjacent row
   is NULL, that side is ignored (edge rows). */
static bool row_ok(int row,
                   const int *above,
                   const int *below) {
    const int *cur = row_digits[row];
    for (int col = 0; col < 3; ++col) {
        int x   = cur[col];
        int s   = MAP[x];
        int cnt = 0;

        if (above && above[col] == s) ++cnt;
        if (below && below[col] == s) ++cnt;
        if (col > 0   && cur[col - 1] == s) ++cnt;
        if (col < 2   && cur[col + 1] == s) ++cnt;

        if (cnt != x) return false;
    }
    return true;
}

/* Build row_digits and transition tables.  This is done only on the
   first call to the sequence function. */
static void build_tables(void) {
    /* Encode all possible rows as base‑5 numbers */
    for (int r = 0; r < MAX_ROWS; ++r) {
        int v = r;
        row_digits[r][2] = v % 5; v /= 5;
        row_digits[r][1] = v % 5; v /= 5;
        row_digits[r][0] = v % 5;
    }

    /* Pre‑compute allowed successors for every pair of rows */
    for (int prev = 0; prev < MAX_ROWS; ++prev) {
        for (int cur = 0; cur < MAX_ROWS; ++cur) {
            int idx = prev * MAX_ROWS + cur;

            /* For each column determine the permitted values of nxt[col] */
            int allowed[3][4];
            int allow_len[3];
            bool impossible = false;

            for (int col = 0; col < 3; ++col) {
                int x = row_digits[cur][col];          /* cell value */
                int s = MAP[x];                       /* required neighbor */
                int f = 0; /* count of known neighbors equal to s */

                /* Left/right neighbors in the same row */
                if (col > 0   && row_digits[cur][col - 1] == s) ++f;
                if (col < 2   && row_digits[cur][col + 1] == s) ++f;

                /* Upper neighbor (we always have one when building a
                   transition, because the state already contains an
                   upper row) */
                if (row_digits[prev][col] == s) ++f;

                /* The lower neighbor is unknown – it will be chosen
                   from the allowed set.  The constraint for this cell
                   is:
                     f + [nxt[col] == s] == x
                */
                if (x < f || x > f + 1) {
                    impossible = true;
                    break;
                } else if (x == f) {
                    /* nxt[col] must NOT equal s: 4 possibilities */
                    int k = 0;
                    for (int v = 0; v < 5; ++v)
                        if (v != s) allowed[col][k++] = v;
                    allow_len[col] = k; /* always 4 */
                } else { /* x == f+1 */
                    /* nxt[col] MUST equal s */
                    allowed[col][0] = s;
                    allow_len[col] = 1;
                }
            }

            if (impossible) {
                trans_len[idx] = 0;
                continue;
            }

            /* Enumerate all combinations of allowed column values
               to form the next row.  Each combination generates
               a distinct successor state. */
            int pos = 0;
            for (int a = 0; a < allow_len[0]; ++a) {
                for (int b = 0; b < allow_len[1]; ++b) {
                    for (int c = 0; c < allow_len[2]; ++c) {
                        int nxt = allowed[0][a] * 25 +
                                  allowed[1][b] * 5 +
                                  allowed[2][c];
                        trans[idx][pos++] = cur * MAX_ROWS + nxt;
                    }
                }
            }
            trans_len[idx] = pos;
        }
    }
    tables_built = true;
}

/* --------------------------------------------------------------------- */
/*  Main sequence routine                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* behaviour below offset is undefined */

    if (!tables_built) build_tables();

    /* Handle the small cases directly */
    if (n == 1) {
        seq_t result = 0;
        for (int r = 0; r < MAX_ROWS; ++r)
            if (row_ok(r, NULL, NULL))
                result += 1;
        return result;
    }

    if (n == 2) {
        seq_t result = 0;
        for (int r1 = 0; r1 < MAX_ROWS; ++r1)
            for (int r2 = 0; r2 < MAX_ROWS; ++r2)
                if (row_ok(r1, NULL, row_digits[r2]) &&
                    row_ok(r2, row_digits[r1], NULL))
                    result += 1;
        return result;
    }

    /* For n >= 3, use DP over pairs of consecutive rows */
    static seq_t dp[MAX_STATES];
    static seq_t ndp[MAX_STATES];
    int i;
    for (i = 0; i < MAX_STATES; ++i) dp[i] = 0;

    /* Initialise states for the first two rows */
    for (int r1 = 0; r1 < MAX_ROWS; ++r1)
        for (int r2 = 0; r2 < MAX_ROWS; ++r2)
            if (row_ok(r1, NULL, row_digits[r2]))
                dp[r1 * MAX_ROWS + r2] += 1;

    /* Each iteration adds one more row */
    for (idx_t step = 3; step <= n; ++step) {
        for (i = 0; i < MAX_STATES; ++i) ndp[i] = 0;

        for (i = 0; i < MAX_STATES; ++i) {
            seq_t cnt = dp[i];
            if (!cnt) continue;
            int prev = i / MAX_ROWS;
            int cur  = i % MAX_ROWS;
            unsigned char k = trans_len[i];
            for (unsigned char t = 0; t < k; ++t)
                ndp[trans[i][t]] += cnt;
        }

        /* swap dp and ndp */
        for (i = 0; i < MAX_STATES; ++i) dp[i] = ndp[i];
    }

    /* Sum over states where the last row is satisfied with its above
       neighbour and no below neighbour. */
    seq_t result = 0;
    for (i = 0; i < MAX_STATES; ++i) {
        int prev = i / MAX_ROWS;
        int cur  = i % MAX_ROWS;
        if (row_ok(cur, row_digits[prev], NULL))
            result += dp[i];
    }
    return result;
}
