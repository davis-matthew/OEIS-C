/*
 * A237370  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median plus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)x4 arrays with entries 0..2 * such
 *            that for every 2x2 subblock the sum of its four entries *
 *            differs from the sums of its horizontal and vertical neighbors *
 *            by exactly one. The values are computed by dynamic programming
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* number of possible rows (4 cells, each 0..2) */
#define NUM_ROWS 81

/* number of valid horizontal sum patterns for a pair of rows:
 * patterns (s0,s1,s2) with abs(s0-s1)=1 and abs(s1-s2)=1, 0<=si<=8  */
#define NUM_PAT 30

/* combined state space: previous row (NUM_ROWS) * pattern id (NUM_PAT) */
#define STATE_COUNT (NUM_ROWS * NUM_PAT)

/* --- Precomputed tables --------------------------------------------- */
static int rows_digits[NUM_ROWS][4];      /* digits of each row */
static int sum_pat[NUM_PAT][3];          /* the 3 sums of a pattern */
static int sum_id_map[9][9][9];          /* map (s0,s1,s2) to pattern id or -1 */
static int pair_s_id[NUM_ROWS][NUM_ROWS];/* pattern id for a pair of rows or -1 */
static bool vertical_ok[NUM_ROWS][NUM_PAT][NUM_ROWS]; /* vertical consistency */

/* Dynamic programming buffers --------------------------------------- */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* Cache of computed a(n) values ------------------------------------- */
static seq_t *cache = NULL;     /* array of cached a(n) */
static idx_t cache_cap = 0;     /* allocated capacity (>=1) */
static idx_t current_max = 0;   /* highest n computed so far */

/* -------------------------------------------------------------------- */
static void init_tables(void)
{
    /* initialise mapping to -1 */
    for (int a = 0; a < 9; ++a)
        for (int b = 0; b < 9; ++b)
            for (int c = 0; c < 9; ++c)
                sum_id_map[a][b][c] = -1;

    /* build rows_digits: represent each row id as base-3 digits */
    for (int r = 0; r < NUM_ROWS; ++r) {
        int tmp = r;
        for (int k = 0; k < 4; ++k) {
            rows_digits[r][k] = tmp % 3;
            tmp /= 3;
        }
    }

    /* build valid sum patterns */
    int pid = 0;
    for (int mid = 0; mid <= 8; ++mid) {
        /* adjacent sums: mid-1 or mid+1, depending on bounds */
        for (int d0 = -1; d0 <= 1; d0 += 2) {
            int s0 = mid + d0;
            if (s0 < 0 || s0 > 8) continue;
            for (int d2 = -1; d2 <= 1; d2 += 2) {
                int s2 = mid + d2;
                if (s2 < 0 || s2 > 8) continue;
                sum_pat[pid][0] = s0;
                sum_pat[pid][1] = mid;
                sum_pat[pid][2] = s2;
                sum_id_map[s0][mid][s2] = pid;
                ++pid;
            }
        }
    }
    /* pid should be NUM_PAT (30) */

    /* compute pattern id for each pair of rows */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            int s[3];
            s[0] = rows_digits[r1][0] + rows_digits[r1][1] + rows_digits[r2][0] + rows_digits[r2][1];
            s[1] = rows_digits[r1][1] + rows_digits[r1][2] + rows_digits[r2][1] + rows_digits[r2][2];
            s[2] = rows_digits[r1][2] + rows_digits[r1][3] + rows_digits[r2][2] + rows_digits[r2][3];
            if (s[0] < 0 || s[0] > 8 || s[1] < 0 || s[1] > 8 || s[2] < 0 || s[2] > 8 ||
                !( (s[0] == s[1] + 1) || (s[0] + 1 == s[1]) ) ||
                !( (s[1] == s[2] + 1) || (s[1] + 1 == s[2]) )) {
                pair_s_id[r1][r2] = -1;
            } else {
                pair_s_id[r1][r2] = sum_id_map[s[0]][s[1]][s[2]];
            }
        }
    }

    /* build vertical_ok table */
    for (int r_prev = 0; r_prev < NUM_ROWS; ++r_prev) {
        for (int pid_prev = 0; pid_prev < NUM_PAT; ++pid_prev) {
            for (int r_next = 0; r_next < NUM_ROWS; ++r_next) {
                int spid = pair_s_id[r_prev][r_next];
                if (spid == -1) {
                    vertical_ok[r_prev][pid_prev][r_next] = false;
                    continue;
                }
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    int diff = sum_pat[pid_prev][k] - sum_pat[spid][k];
                    if (diff != 1 && diff != -1) {
                        ok = false;
                        break;
                    }
                }
                vertical_ok[r_prev][pid_prev][r_next] = ok;
            }
        }
    }

    /* allocate DP buffers */
    dp_cur  = (seq_t *)calloc((size_t)STATE_COUNT, sizeof(seq_t));
    dp_next = (seq_t *)calloc((size_t)STATE_COUNT, sizeof(seq_t));
}

/* Ensure cache array can hold index n */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= n + 1) new_cap *= 2;
    seq_t *new_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, (size_t)cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 */

    if (!dp_cur) init_tables();

    /* compute up to n if needed */
    if (n > current_max) {
        /* initialise for n = 1 if not yet done */
        if (current_max == 0) {
            for (int r0 = 0; r0 < NUM_ROWS; ++r0) {
                for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
                    int pid = pair_s_id[r0][r1];
                    if (pid != -1) {
                        idx_t state = (idx_t)r1 * NUM_PAT + pid;
                        dp_cur[state] += 1;
                    }
                }
            }
            seq_t total = 0;
            for (size_t idx = 0; idx < STATE_COUNT; ++idx) total += dp_cur[idx];
            ensure_cache_size(1);
            cache[1] = total;
            current_max = 1;
        }

        /* extend DP to n */
        for (idx_t step = current_max + 1; step <= n; ++step) {
            /* zero dp_next */
            memset(dp_next, 0, (size_t)STATE_COUNT * sizeof(seq_t));

            /* transition */
            for (int prev_row = 0; prev_row < NUM_ROWS; ++prev_row) {
                for (int pid_prev = 0; pid_prev < NUM_PAT; ++pid_prev) {
                    idx_t cur_state = (idx_t)prev_row * NUM_PAT + pid_prev;
                    seq_t cnt = dp_cur[cur_state];
                    if (cnt == 0) continue;
                    for (int next_row = 0; next_row < NUM_ROWS; ++next_row) {
                        if (!vertical_ok[prev_row][pid_prev][next_row]) continue;
                        int spid = pair_s_id[prev_row][next_row];
                        if (spid == -1) continue;
                        idx_t next_state = (idx_t)next_row * NUM_PAT + spid;
                        dp_next[next_state] += cnt;
                    }
                }
            }

            /* swap buffers */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;

            /* compute a(step) */
            seq_t total = 0;
            for (size_t idx = 0; idx < STATE_COUNT; ++idx) total += dp_cur[idx];
            ensure_cache_size(step);
            cache[step] = total;
            current_max = step;
        }
    }

    return cache[n];
}
