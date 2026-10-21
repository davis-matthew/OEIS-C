/*
 * A237726  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237726: Number of (n+1)×5 arrays over {0,1,2} such that the value
   (max + upper median – lower median) of every 2×2 subblock differs by
   exactly 1 from its horizontal and vertical neighbours.  The DP below
   stores, for each ordered pair of consecutive rows, the number of ways
   to build the rows seen so far with those two rows as the bottom two.
   The answer for index n (rows = n+1) is the sum of the counts for all
   states after building n+1 rows.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROWS 243            /* 3^5 possible rows */
#define NUM_STATES (NUM_ROWS * NUM_ROWS)

/* Representation of all 3^5 rows.  row_digits[r][c] gives the digit in
   column c of row r (0≤c<5).  */
static uint8_t row_digits[NUM_ROWS][5];
/* For each ordered pair (a,b) the array of the four f-values obtained from
   the 2×2 blocks of rows a and b.  */
static uint8_t fvals[NUM_STATES][4];
/* If a state has horizontal adjacency satisfied.  */
static uint8_t horiz_flag[NUM_STATES];
/* adjacency list for each state: all states (b,c) that can follow (a,b).  */
static uint16_t *adj[NUM_STATES];
static uint16_t   deg[NUM_STATES];

/* DP arrays.  dp_cur[state] = number of arrays ending with that state
   after processing the current number of rows.  */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
/* Lists of active states for quick iteration.  */
static int *active_cur = NULL;
static int *active_next = NULL;
/* Flag array to avoid duplicating states in the active_next list.  */
static uint8_t *written = NULL;
static size_t active_cur_sz  = 0;
static size_t active_next_sz = 0;

/* Cached answers: answers[k] = a(k+1).  */
static seq_t *answers = NULL;
static size_t answers_cap = 0;
static size_t answers_size = 0;

static void decode_row(int idx, uint8_t out[5]) {
    for (int i = 0; i < 5; ++i) {
        out[i] = idx % 3;
        idx /= 3;
    }
}

static void compute_f(const uint8_t a[5], const uint8_t b[5], uint8_t out[4]) {
    for (int j = 0; j < 4; ++j) {
        int x0 = a[j];
        int x1 = a[j + 1];
        int x2 = b[j];
        int x3 = b[j + 1];
        /* sorting network to find max, upper median, lower median */
        if (x1 < x0) { int t = x0; x0 = x1; x1 = t; }
        if (x3 < x2) { int t = x2; x2 = x3; x3 = t; }
        if (x2 < x0) { int t = x0; x0 = x2; x2 = t; }
        if (x3 < x1) { int t = x1; x1 = x3; x3 = t; }
        if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
        /* f = max + upper_median - lower_median */
        out[j] = (uint8_t)(x3 + x2 - x1);
    }
}

static bool horiz_ok(const uint8_t f[4]) {
    for (int j = 0; j < 3; ++j)
        if (abs((int)f[j] - (int)f[j + 1]) != 1)
            return false;
    return true;
}

/* Initialise all global tables and the first answer.  */
static bool initialized = false;
static void init(void) {
    for (int r = 0; r < NUM_ROWS; ++r)
        decode_row(r, row_digits[r]);

    for (int a = 0; a < NUM_ROWS; ++a) {
        for (int b = 0; b < NUM_ROWS; ++b) {
            int idx = a * NUM_ROWS + b;
            compute_f(row_digits[a], row_digits[b], fvals[idx]);
            horiz_flag[idx] = horiz_ok(fvals[idx]) ? 1 : 0;
        }
    }

    /* Build adjacency lists */
    for (int idx = 0; idx < NUM_STATES; ++idx) {
        if (!horiz_flag[idx]) {
            deg[idx] = 0;
            adj[idx] = NULL;
            continue;
        }
        int a = idx / NUM_ROWS;
        int b = idx % NUM_ROWS;
        uint8_t *fa = fvals[idx];
        int cnt = 0;
        for (int c = 0; c < NUM_ROWS; ++c) {
            int next_idx = b * NUM_ROWS + c;
            if (!horiz_flag[next_idx]) continue;
            uint8_t *fb = fvals[next_idx];
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (abs((int)fa[k] - (int)fb[k]) != 1) {
                    ok = false; break;
                }
            }
            if (ok) ++cnt;
        }
        deg[idx] = (uint16_t)cnt;
        if (cnt > 0) {
            adj[idx] = (uint16_t *)malloc(cnt * sizeof(uint16_t));
            int pos = 0;
            for (int c = 0; c < NUM_ROWS; ++c) {
                int next_idx = b * NUM_ROWS + c;
                if (!horiz_flag[next_idx]) continue;
                uint8_t *fb = fvals[next_idx];
                bool ok = true;
                for (int k = 0; k < 4; ++k) {
                    if (abs((int)fa[k] - (int)fb[k]) != 1) { ok = false; break; }
                }
                if (ok) adj[idx][pos++] = (uint16_t)next_idx;
            }
        } else
            adj[idx] = NULL;
    }

    dp_cur  = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));
    dp_next = (seq_t *)calloc(NUM_STATES, sizeof(seq_t));

    active_cur  = (int *)malloc(NUM_STATES * sizeof(int));
    active_next = (int *)malloc(NUM_STATES * sizeof(int));

    written = (uint8_t *)calloc(NUM_STATES, sizeof(uint8_t));

    /* initialise first DP (two rows) */
    active_cur_sz = 0;
    for (int idx = 0; idx < NUM_STATES; ++idx) {
        if (horiz_flag[idx]) {
            dp_cur[idx] = (seq_t)1;
            active_cur[active_cur_sz++] = idx;
        }
    }

    /* cache first answer */
    answers_cap = 64;
    answers = (seq_t *)malloc(answers_cap * sizeof(seq_t));
    answers_size = 0;
    answers[answers_size++] = (seq_t)active_cur_sz;   /* a(1) */

    initialized = true;
}

static void ensure_answers_cap(size_t needed) {
    if (needed <= answers_cap) return;
    size_t newcap = answers_cap * 2;
    if (newcap < needed) newcap = needed;
    answers = (seq_t *)realloc(answers, newcap * sizeof(seq_t));
    answers_cap = newcap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but safe */
    if (!initialized) init();

    while (answers_size < n) {
        /* zero next DP array */
        memset(dp_next, 0, NUM_STATES * sizeof(seq_t));
        active_next_sz = 0;

        /* accumulate counts for next state list and sum of all next states */
        seq_t sum_next = 0;
        for (size_t p = 0; p < active_cur_sz; ++p) {
            int idx = active_cur[p];
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            uint16_t *list = adj[idx];
            int d = deg[idx];
            for (int q = 0; q < d; ++q) {
                int next = list[q];
                if (!written[next]) {
                    written[next] = 1;
                    active_next[active_next_sz++] = next;
                }
                dp_next[next] += val;
                sum_next += val;
            }
        }

        /* reset written flags */
        for (size_t p = 0; p < active_next_sz; ++p)
            written[active_next[p]] = 0;

        /* store result for this n */
        ensure_answers_cap(answers_size + 1);
        answers[answers_size++] = sum_next;

        /* swap dp arrays and active state lists */
        seq_t *tmp_dp = dp_cur; dp_cur = dp_next; dp_next = tmp_dp;

        int *tmp_act = active_cur; active_cur = active_next; active_next = tmp_act;
        size_t tmp_sz = active_cur_sz; active_cur_sz = active_next_sz; active_next_sz = tmp_sz;
    }

    return answers[n - 1];
}
