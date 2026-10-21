/*
 * A237571  Number of (n+1)X(3+1) 0..3 arrays with the upper median minus the
 *          lower median of every 2X2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A237571
 *  Count of (n+1)×4 arrays over {0,1,2,3} such that for every 2×2 subblock
 *  the difference (upper median – lower median) differs from its
 *  horizontal and vertical neighbors by exactly one.
 *
 *  The algorithm works by
 *  – precomputing all 4‑digit rows and the 3‑digit median differences
 *  – building a directed graph of rows whose edge labels are the
 *  3‑vector of median differences; only those edges that satisfy the
 *  horizontal adjacency property are kept;
 *  – performing a DP over rows where adjacency of median difference
 *    vectors in successive pairs is enforced (|Δ|=1 in each of 3 columns).
 *  The number of valid sequences of rows of length n+1 equals a(n).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_ROWS 256
#define MAX_PAT  10
#define PAT_ID_NOT 255U

typedef idx_t uix;   /* for readability */

/* Precomputed data */
static uint8_t row_cells[MAX_ROWS][4];           /* 4 digits for each 0..255 */
static uint8_t pair_pat[MAX_ROWS][MAX_ROWS];    /* pattern id or PAT_ID_NOT */
static uint8_t neigh_cnt[MAX_ROWS][MAX_PAT];
static uint8_t neigh_list[MAX_ROWS][MAX_PAT][MAX_ROWS];
static uint8_t allowed[MAX_PAT][MAX_PAT];
static uint8_t pattern[MAX_PAT][3];
static unsigned pattern_cnt;

/* ---------- helper to initialise tables ---------- */
static void init_tables(void)
{
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    /* 1. build row_cells */
    for (uix r = 0; r < MAX_ROWS; ++r) {
        uix v = r;
        for (int j = 0; j < 4; ++j) {
            row_cells[r][j] = v & 3;
            v >>= 2;
        }
    }

    /* 2. enumerate all 3‑digit patterns with abs differences 1 */
    pattern_cnt = 0;
    for (int d0 = 0; d0 <= 3; ++d0)
        for (int d1 = 0; d1 <= 3; ++d1)
            for (int d2 = 0; d2 <= 3; ++d2) {
                if (abs(d0 - d1) == 1 && abs(d1 - d2) == 1) {
                    pattern[pattern_cnt][0] = d0;
                    pattern[pattern_cnt][1] = d1;
                    pattern[pattern_cnt][2] = d2;
                    ++pattern_cnt;
                }
            }
    /* pattern_cnt should be 10 */

    /* 3. allowed matrix */
    for (int p = 0; p < pattern_cnt; ++p)
        for (int q = 0; q < pattern_cnt; ++q) {
            bool ok = true;
            for (int k = 0; k < 3; ++k)
                if (abs((int)pattern[p][k] - (int)pattern[q][k]) != 1) {
                    ok = false;
                    break;
                }
            allowed[p][q] = ok;
        }

    /* 4. pair_pat and neigh_list */
    for (uix r = 0; r < MAX_ROWS; ++r) {
        for (uix s = 0; s < MAX_ROWS; ++s) {
            /* compute 3 differences for rows r (top) and s (bottom) */
            uint8_t val[4];
            uint8_t d[3];
            for (int j = 0; j < 3; ++j) {
                /* 4 cells for this 2×2 block */
                val[0] = row_cells[r][j];
                val[1] = row_cells[r][j + 1];
                val[2] = row_cells[s][j];
                val[3] = row_cells[s][j + 1];
                /* simple sorting network for 4 values */
                if (val[0] > val[1]) { uint8_t t = val[0]; val[0] = val[1]; val[1] = t; }
                if (val[2] > val[3]) { uint8_t t = val[2]; val[2] = val[3]; val[3] = t; }
                if (val[0] > val[2]) { uint8_t t = val[0]; val[0] = val[2]; val[2] = t; }
                if (val[1] > val[3]) { uint8_t t = val[1]; val[1] = val[3]; val[3] = t; }
                if (val[1] > val[2]) { uint8_t t = val[1]; val[1] = val[2]; val[2] = t; }
                d[j] = val[2] - val[1];  /* upper median - lower median */
            }
            /* check horizontal adjacency within this pair */
            if (abs((int)d[0] - (int)d[1]) == 1 && abs((int)d[1] - (int)d[2]) == 1) {
                /* find pattern id */
                int pid = PAT_ID_NOT;
                for (int p = 0; p < pattern_cnt; ++p)
                    if (pattern[p][0] == d[0] && pattern[p][1] == d[1] && pattern[p][2] == d[2]) {
                        pid = p;
                        break;
                    }
                pair_pat[r][s] = (uint8_t)pid;
                if (pid < MAX_PAT) {
                    neigh_list[r][pid][neigh_cnt[r][pid]++] = s;
                }
            } else {
                pair_pat[r][s] = PAT_ID_NOT;
            }
        }
    }
}

/* ---------- the main function ---------- */
seq_t a(idx_t n)
{
    /* n is 1‑based index: a(1) is first term */
    init_tables();

    /* Two DP layers: previous and current */
    static seq_t dp_prev[MAX_ROWS][MAX_PAT];
    static seq_t dp_curr[MAX_ROWS][MAX_PAT];
    /* Clear previous layer */
    for (int i = 0; i < MAX_ROWS; ++i)
        for (int j = 0; j < MAX_PAT; ++j)
            dp_prev[i][j] = 0;

    /* Base case: n = 1 → sequences of 2 rows */
    for (int r0 = 0; r0 < MAX_ROWS; ++r0) {
        for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
            uint8_t pid = pair_pat[r0][r1];
            if (pid != PAT_ID_NOT) {
                dp_prev[r1][pid] = dp_prev[r1][pid] + 1;
            }
        }
    }

    /* If only one pair needed, sum and return */
    if (n == 1)
    {
        seq_t sum = 0;
        for (int r = 0; r < MAX_ROWS; ++r)
            for (int p = 0; p < MAX_PAT; ++p)
                sum = sum + dp_prev[r][p];
        return sum;
    }

    /* For greater n, iterate rows */
    for (uix step = 2; step <= n; ++step)
    {
        /* Zero current layer */
        for (int i = 0; i < MAX_ROWS; ++i)
            for (int j = 0; j < MAX_PAT; ++j)
                dp_curr[i][j] = 0;

        /* Transition */
        for (int r = 0; r < MAX_ROWS; ++r)
        {
            for (int p_prev = 0; p_prev < MAX_PAT; ++p_prev)
            {
                seq_t cnt = dp_prev[r][p_prev];
                if (!cnt) continue;
                for (int p_curr = 0; p_curr < MAX_PAT; ++p_curr)
                {
                    if (!allowed[p_prev][p_curr]) continue;
                    int nbk = neigh_cnt[r][p_curr];
                    uint8_t *nbptr = neigh_list[r][p_curr];
                    for (int k = 0; k < nbk; ++k)
                    {
                        int next = nbptr[k];
                        dp_curr[next][p_curr] = dp_curr[next][p_curr] + cnt;
                    }
                }
            }
        }

        /* Swap layers */
        for (int i = 0; i < MAX_ROWS; ++i)
            for (int j = 0; j < MAX_PAT; ++j)
                dp_prev[i][j] = dp_curr[i][j];
    }

    /* Sum all final states */
    seq_t total = 0;
    for (int r = 0; r < MAX_ROWS; ++r)
        for (int p = 0; p < MAX_PAT; ++p)
            total = total + dp_prev[r][p];

    return total;
}
