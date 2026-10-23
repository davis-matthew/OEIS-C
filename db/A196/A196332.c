/*
 * A196332  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,0,3,1,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via a linear recurrence of order 17 derived from
 *            the empirical relation in OEIS A196332
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 125
#define COLS 3

/* helper arrays */
static uint8_t rows[ROWS][COLS];
static unsigned char target[5] = {4,0,3,1,2};

/* build all possible rows */
static void init_rows(void)
{
    static int done = 0;
    if (done) return;
    int idx = 0;
    for (int a = 0; a <= 4; ++a)
        for (int b = 0; b <= 4; ++b)
            for (int c = 0; c <= 4; ++c) {
                rows[idx][0] = a;
                rows[idx][1] = b;
                rows[idx][2] = c;
                ++idx;
            }
    done = 1;
}

/* check a single row for n = 1 (no neighbors) */
static int check_single(const uint8_t *row)
{
    for (int j = 0; j < COLS; ++j) {
        uint8_t x = row[j];
        uint8_t t = target[x];
        int cnt = 0;
        if (j > 0 && row[j-1] == t) ++cnt;
        if (j < COLS-1 && row[j+1] == t) ++cnt;
        if (cnt != x) return 0;
    }
    return 1;
}

/* check top row (no up neighbor, down neighbor exists) */
static int check_top(const uint8_t *row, const uint8_t *down)
{
    for (int j = 0; j < COLS; ++j) {
        uint8_t x = row[j];
        uint8_t t = target[x];
        int cnt = 0;
        if (j > 0 && row[j-1] == t) ++cnt;
        if (j < COLS-1 && row[j+1] == t) ++cnt;
        if (down[j] == t) ++cnt;
        if (cnt != x) return 0;
    }
    return 1;
}

/* check bottom row (no down neighbor, up neighbor exists) */
static int check_bottom(const uint8_t *row, const uint8_t *up)
{
    for (int j = 0; j < COLS; ++j) {
        uint8_t x = row[j];
        uint8_t t = target[x];
        int cnt = 0;
        if (j > 0 && row[j-1] == t) ++cnt;
        if (j < COLS-1 && row[j+1] == t) ++cnt;
        if (up[j] == t) ++cnt;
        if (cnt != x) return 0;
    }
    return 1;
}

/* check middle row (has both up and down neighbors) */
static int check_middle(const uint8_t *row,
                        const uint8_t *up,
                        const uint8_t *down)
{
    for (int j = 0; j < COLS; ++j) {
        uint8_t x = row[j];
        uint8_t t = target[x];
        int cnt = 0;
        if (j > 0 && row[j-1] == t) ++cnt;
        if (j < COLS-1 && row[j+1] == t) ++cnt;
        if (up[j] == t) ++cnt;
        if (down[j] == t) ++cnt;
        if (cnt != x) return 0;
    }
    return 1;
}

/* compute first 17 terms of the sequence */
static void compute_first_terms(seq_t *term)
{
    init_rows();

    /* auxiliary arrays */
    static int top_ok[ROWS][ROWS];
    static unsigned char trans_ok[ROWS][ROWS][ROWS];
    int i;

    /* build top_ok */
    for (int r1 = 0; r1 < ROWS; ++r1)
        for (int r2 = 0; r2 < ROWS; ++r2)
            top_ok[r1][r2] = check_top(rows[r1], rows[r2]);

    /* build trans_ok */
    for (int pp = 0; pp < ROWS; ++pp)
        for (int p = 0; p < ROWS; ++p)
            for (int cur = 0; cur < ROWS; ++cur)
                trans_ok[pp][p][cur] =
                    check_middle(rows[p], rows[pp], rows[cur]);

    /* a(1) */
    term[1] = 0;
    for (int r = 0; r < ROWS; ++r)
        if (check_single(rows[r]))
            term[1] += 1;

    /* a(2) */
    term[2] = 0;
    for (int r1 = 0; r1 < ROWS; ++r1)
        for (int r2 = 0; r2 < ROWS; ++r2)
            if (top_ok[r1][r2] && check_bottom(rows[r2], rows[r1]))
                term[2] += 1;

    /* DP arrays for n >= 3 */
    static seq_t cur_dp[ROWS*ROWS];
    static seq_t next_dp[ROWS*ROWS];

    memset(cur_dp, 0, sizeof(cur_dp));

    /* initial states after adding second row */
    for (int r1 = 0; r1 < ROWS; ++r1)
        for (int r2 = 0; r2 < ROWS; ++r2)
            if (top_ok[r1][r2])
                cur_dp[r1*ROWS + r2] = 1;

    for (i = 3; i <= 17; ++i) {
        memset(next_dp, 0, sizeof(next_dp));
        for (int idx = 0; idx < ROWS*ROWS; ++idx) {
            seq_t cnt = cur_dp[idx];
            if (cnt == 0) continue;
            int pp = idx / ROWS;
            int p  = idx % ROWS;
            for (int cur = 0; cur < ROWS; ++cur)
                if (trans_ok[pp][p][cur])
                    next_dp[p*ROWS + cur] += cnt;
        }
        memcpy(cur_dp, next_dp, sizeof(cur_dp));

        /* compute term i from bottom row validation */
        seq_t total = 0;
        for (int idx = 0; idx < ROWS*ROWS; ++idx) {
            seq_t cnt = cur_dp[idx];
            if (cnt == 0) continue;
            int pp = idx / ROWS;
            int p  = idx % ROWS;
            if (check_bottom(rows[p], rows[pp]))
                total += cnt;
        }
        term[i] = total;
    }
}

/* recurrence coefficients and offsets */
static const int coef[17] = {
    3, -1, -5, 6, 6, -22, -71, 6, 95, 3,
    139, -20, -45, 44, -6, -14
};

static const int offset[17] = {
    1, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17
};

seq_t a(idx_t n)
{
    /* lazy static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 17;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        compute_first_terms(cache);
    }

    if (n <= cache_size) return cache[n];

    /* extend cache up to n */
    if (n > cache_size) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 0; k < 17; ++k) {
                int c = coef[k];
                idx_t idx = i - offset[k];
                const seq_t val = cache[idx];
                if (c > 0)
                    sum_pos += (seq_t)c * val;
                else
                    sum_neg += (seq_t)(-c) * val;
            }
            cache[i] = sum_pos - sum_neg;  /* sum_pos >= sum_neg */
        }
        cache_size = n;
    }
    return cache[n];
}
