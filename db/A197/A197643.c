/*
 * A197643  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,2,2,0,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by transfer‑matrix DP on 3‑column arrays.
 *            Recurrence: a(n) = a(n‑1) + … (but we compute directly). For n ≥
 *            1 the value a(n) is the number of n×3 arrays over {0,…,4}
 *            satisfying the neighbour constraints described in OEIS A197643
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define COLS 3
#define VALS 5
#define PATTS 125          /* 5^3 possible rows */
#define SENTINEL 125       /* index for “no neighbour” */

/* mapping from pattern index to its 3 cell values (base‑5 representation) */
static int8_t rowval[PATTS][COLS];

/* for sentinel pattern, values are irrelevant (-1) */
static int8_t rowval_sentinel[COLS] = {-1, -1, -1};

/* trans_len[state] = number of rows that can follow this state
   trans_list[state][k] the k‑th allowed row index (0..124) */
static uint8_t trans_len[PATTS * PATTS];
static uint16_t trans_list[PATTS * PATTS][PATTS];

/* target value and required count for each cell value (0..4) */
static const int8_t target[VALS] = {4, 2, 2, 0, 2};
static const uint8_t req_cnt[VALS] = {0, 1, 2, 3, 4};

/* helper: check whether a middle row `mid` satisfies its constraints
   given optional `up` and `down` rows; if up/down == SENTINEL, that
   neighbour is absent.  Returns 1 if constraints are met, 0 otherwise. */
static int check_row(int mid, int up, int down)
{
    const int8_t *midv = (const int8_t *)rowval[mid];
    const int8_t *upv  = (up == SENTINEL) ? rowval_sentinel : (const int8_t *)rowval[up];
    const int8_t *downv= (down == SENTINEL) ? rowval_sentinel : (const int8_t *)rowval[down];

    for (int j = 0; j < COLS; ++j) {
        int8_t x = midv[j];
        int v = target[x];
        uint8_t need = req_cnt[x];
        uint8_t cnt = 0;

        /* up neighbour */
        if (up != SENTINEL && upv[j] == v) ++cnt;
        /* down neighbour */
        if (down != SENTINEL && downv[j] == v) ++cnt;
        /* left neighbour */
        if (j > 0 && midv[j-1] == v) ++cnt;
        /* right neighbour */
        if (j < COLS-1 && midv[j+1] == v) ++cnt;

        if (cnt != need) return 0;
    }
    return 1;
}

/* initialize tables used by the DP */
static void init_tables(void)
{
    /* encode all patterns */
    for (int p = 0; p < PATTS; ++p) {
        int n = p;
        for (int i = COLS - 1; i >= 0; --i) {
            rowval[p][i] = n % VALS;
            n /= VALS;
        }
    }

    /* precompute allowed next rows for each state (up,mid) */
    for (int up = 0; up < PATTS; ++up) {
        for (int mid = 0; mid < PATTS; ++mid) {
            int state = up * PATTS + mid;
            uint8_t len = 0;
            for (int cur = 0; cur < PATTS; ++cur) {
                if (check_row(mid, up, cur)) {
                    trans_list[state][len++] = cur;
                }
            }
            trans_len[state] = len;
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* the tables must be built once */
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    if (n == 0) return 0;

    /* arrays for DP */
    static seq_t dp[PATTS * PATTS];
    static seq_t dp_next[PATTS * PATTS];
    for (int i = 0; i < PATTS * PATTS; ++i) dp[i] = 0;

    seq_t answer = 0;

    /* n = 1 : single row, only check its own constraints */
    if (n == 1) {
        for (int p = 0; p < PATTS; ++p) {
            if (check_row(p, SENTINEL, SENTINEL))
                answer += (seq_t)1;
        }
        return answer;
    }

    /* initialize with all first two rows that satisfy the first row's constraints */
    for (int first = 0; first < PATTS; ++first) {
        for (int second = 0; second < PATTS; ++second) {
            if (check_row(first, SENTINEL, second))
                dp[first * PATTS + second] = 1;
        }
    }

    /* DP over remaining rows (from 3rd row up to nth row) */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < PATTS * PATTS; ++i) dp_next[i] = 0;
        for (int state = 0; state < PATTS * PATTS; ++state) {
            seq_t ways = dp[state];
            if (ways == 0) continue;
            uint8_t len = trans_len[state];
            int up   = state / PATTS;
            int mid  = state % PATTS;
            for (uint8_t k = 0; k < len; ++k) {
                int cur = trans_list[state][k];
                int nxt = mid * PATTS + cur;
                dp_next[nxt] += ways;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < PATTS * PATTS; ++i) dp[i] = dp_next[i];
    }

    /* final row: check last row's constraints with no down neighbour */
    for (int state = 0; state < PATTS * PATTS; ++state) {
        int up  = state / PATTS;
        int mid = state % PATTS;
        if (check_row(mid, up, SENTINEL))
            answer += dp[state];
    }

    return answer;
}
