/*
 * A237467  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of (n+1)×4 0..2 arrays such that for every 2×2
 * subblock the value max + lower median – upper median differs by exactly
 * one from its horizontal and vertical neighboring subblocks.
 *
 * Computation: dynamic programming over rows of the array.  For each
 * subblock row we store a state consisting of the f‑values (3 cells)
 * of that row and the previous row of the original array.  The
 * transition tables are pre‑computed once.  From the DP we obtain
 * a(1) and subsequently a(2), a(3), … .
 * The sequence values are cached; subsequent calls for larger n use
 * the cached results and continuously extend the DP.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* constants */
#define ROWS 81              /* 3^4 possible 4‑cell rows of the array   */
#define FEDED 27             /* 3^3 possible 3‑cell f‑value rows        */
#define STATE_COUNT 2187     /* ROWS * FEDED                           */
#define INIT_CACHE_CAP 32    /* initial cache size                      */

/* state tables ------------------------------------------------------------ */
static uint8_t row_digits[ROWS][4];        /* base‑3 digits of a row             */
static uint8_t f_to_idx[3][3][3];
static uint8_t idx_to_f[FEDED][3];
static bool vertical_ok[FEDED][FEDED];
static int8_t next_f[ROWS][ROWS];         /* -1 if the pair is invalid         */
static uint8_t next_row_list[ROWS][ROWS]; /* list of valid next rows per row    */
static uint8_t next_fidx_list[ROWS][ROWS];
static uint8_t next_count[ROWS];

/* DP arrays -------------------------------------------------------------- */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* cache of a(n) ---------------------------------------------------------- */
static seq_t *cache = NULL;   /* a(0) unused, a(1) at index 1          */
static size_t cache_used = 0; /* largest n for which a(n) is stored   */
static size_t cache_cap  = 0;

/* pre‑computation --------------------------------------------------------- */
static void precompute(void)
{
    /* row digits in base‑3 */
    for (int idx = 0; idx < ROWS; ++idx) {
        int x = idx;
        for (int j = 0; j < 4; ++j) {
            row_digits[idx][j] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* f ↔ index maps */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                int idx = a + 3 * b + 9 * c;
                f_to_idx[a][b][c] = (uint8_t)idx;
                idx_to_f[idx][0] = (uint8_t)a;
                idx_to_f[idx][1] = (uint8_t)b;
                idx_to_f[idx][2] = (uint8_t)c;
            }

    /* vertical adjacency table */
    for (int p = 0; p < FEDED; ++p)
        for (int q = 0; q < FEDED; ++q) {
            bool ok = true;
            for (int k = 0; k < 3 && ok; ++k)
                ok = abs((int)idx_to_f[p][k] - (int)idx_to_f[q][k]) == 1;
            vertical_ok[p][q] = ok;
        }

    /* transition tables */
    memset(next_f, -1, sizeof(next_f));
    memset(next_count, 0, sizeof(next_count));

    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int f_val[3];
            bool ok = true;
            for (int col = 0; col < 3; ++col) {
                int a = row_digits[r1][col];
                int b = row_digits[r1][col + 1];
                int c = row_digits[r2][col];
                int d = row_digits[r2][col + 1];
                /* compute f for this 2×2 block */
                int v[4] = {a, b, c, d};
                /* insertion sort of 4 elements */
                for (int i = 1; i < 4; ++i) {
                    int x = v[i], j = i - 1;
                    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; --j; }
                    v[j + 1] = x;
                }
                int max_val      = v[3];
                int lower_med    = v[1];
                int upper_med    = v[2];
                int f = max_val + lower_med - upper_med;
                if (f < 0 || f > 2) { ok = false; break; }
                f_val[col] = f;
            }
            if (!ok) continue;
            if (abs(f_val[0] - f_val[1]) != 1 || abs(f_val[1] - f_val[2]) != 1)
                continue;
            int fidx = f_val[0] + 3 * f_val[1] + 9 * f_val[2];
            next_f[r1][r2] = (int8_t)fidx;
            uint8_t c = next_count[r1]++;
            next_row_list[r1][c] = (uint8_t)r2;
            next_fidx_list[r1][c] = (uint8_t)fidx;
        }
    }

    /* DP arrays */
    dp_cur  = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));

    /* a(1)  – initialise dp_cur from all possible pairs of rows */
    for (int r1 = 0; r1 < ROWS; ++r1)
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int8_t fidx = next_f[r1][r2];
            if (fidx < 0) continue;
            size_t state = ((size_t)fidx) * ROWS + r2;
            dp_cur[state] += (seq_t)1;
        }

    /* cache */
    cache = (seq_t *)calloc(INIT_CACHE_CAP, sizeof(seq_t));
    cache_cap = INIT_CACHE_CAP;
    cache_used = 1;   /* a(1) will be stored next */

    /* compute a(1) */
    seq_t sum = (seq_t)0;
    for (size_t s = 0; s < STATE_COUNT; ++s)
        sum += dp_cur[s];
    if (cache_cap <= 1) {
        cache_cap = 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[1] = sum;
}

/* compute a(n) ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (dp_cur == NULL) precompute();

    if (n <= cache_used) return cache[n];

    /* extend cache/DP up to n */
    for (idx_t cur = cache_used; cur < n; ++cur) {
        /* dp_next = 0 */
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        /* transitions */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;

            uint8_t prev_f = (uint8_t)(s / ROWS);
            uint8_t prev_row = (uint8_t)(s % ROWS);

            for (uint8_t k = 0; k < next_count[prev_row]; ++k) {
                uint8_t curr_row = next_row_list[prev_row][k];
                uint8_t curr_f   = next_fidx_list[prev_row][k];

                if (!vertical_ok[prev_f][curr_f]) continue;

                size_t new_state = ((size_t)curr_f) * ROWS + curr_row;
                dp_next[new_state] += val;
            }
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* compute a(cur+1) */
        seq_t total = (seq_t)0;
        for (size_t s = 0; s < STATE_COUNT; ++s)
            total += dp_cur[s];

        /* enlarge cache if needed */
        if (cache_used + 1 >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cache[cur + 1] = total;
        ++cache_used;
    }

    return cache[n];
}
