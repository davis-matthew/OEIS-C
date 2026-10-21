/*
 * A234918  Number of (n+1) X (5+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays over {0,…,4} with (1) no
 *            horizontally adjacent equal entries in a row, (2) no vertically
 *            adjacent equal entries in a column, (3) for every 2×2 subblock
 *            the absolute difference between diagonal sums equals 2. Computed
 *            by dynamic programming on admissible rows and th...
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BASE 5                /* digits 0..4 */
#define COLS 6                /* 5+1 columns */
#define MAX_STATES (BASE * (BASE-1)*(BASE-1)*(BASE-1)*(BASE-1)*(BASE-1)) /* 5*4^5 = 5120 */

static bool initialized = false;
static size_t n_states = 0;          /* number of admissible rows */

static uint8_t *state_digits = NULL;    /* flattened n_states × COLS array */
static uint32_t *out_start = NULL;      /* start index in out_dest for each state */
static uint16_t *out_cnt = NULL;        /* outgoing degree for each state */
static uint32_t *out_dest = NULL;       /* concatenated destination lists */

static seq_t dp_cur[MAX_STATES];
static seq_t dp_next[MAX_STATES];
static idx_t current_n = 0;              /* number of processed rows */

/* ------------------------------------------------------------------
 * Helper: test whether two rows can be stacked respecting all constraints.
 */
static inline bool valid_transition(const uint8_t *a, const uint8_t *b)
{
    size_t i;
    /* Vertical adjacency: no equal elements in same column */
    for (i = 0; i < COLS; ++i)
        if (a[i] == b[i]) return false;

    /* Diagonal‑sum difference of every 2×2 block must be exactly 2 */
    for (i = 0; i < COLS - 1; ++i) {
        int diff = (int)(a[i] + b[i + 1]) - (int)(b[i] + a[i + 1]);
        if (diff < 0) diff = -diff;
        if (diff != 2) return false;
    }
    return true;
}

/* ------------------------------------------------------------------
 * Build all admissible rows and the transition graph.
 */
static void init(void)
{
    if (initialized) return;

    /* enumerate all 5^6 strings of length 6 and keep the admissible ones */
    uint64_t total_numbers = 1;
    for (size_t i = 0; i < COLS; ++i) total_numbers *= BASE;   /* 5^6 = 15625 */

    /* temporary buffer for all candidates (flat array) */
    uint8_t *all_digits = (uint8_t*)malloc(total_numbers * COLS);
    if (!all_digits) exit(EXIT_FAILURE);

    /* state_id maps candidate number to row index or -1 if invalid */
    uint16_t *state_id = (uint16_t*)malloc(total_numbers * sizeof(uint16_t));
    if (!state_id) exit(EXIT_FAILURE);

    n_states = 0;
    for (uint64_t num = 0; num < total_numbers; ++num) {
        uint64_t tmp = num;
        bool ok = true;
        for (int d = COLS - 1; d >= 0; --d) {
            uint8_t val = tmp % BASE;
            tmp /= BASE;
            all_digits[num * COLS + d] = val;
            if (d < COLS - 1 && all_digits[num * COLS + d] == all_digits[num * COLS + d + 1])
                ok = false;
        }
        if (ok) {
            state_id[num] = (uint16_t)n_states++;
        } else {
            state_id[num] = (uint16_t)(-1);
        }
    }

    /* allocate and fill the flattened array of admissible rows */
    state_digits = (uint8_t*)malloc(n_states * COLS);
    if (!state_digits) exit(EXIT_FAILURE);
    for (uint64_t num = 0; num < total_numbers; ++num) {
        if (state_id[num] != (uint16_t)(-1)) {
            size_t idx = state_id[num];
            memcpy(state_digits + idx * COLS, all_digits + num * COLS, COLS);
        }
    }
    free(all_digits);
    free(state_id);

    /* count outgoing edges for each state */
    out_cnt = (uint16_t*)calloc(n_states, sizeof(uint16_t));
    if (!out_cnt) exit(EXIT_FAILURE);

    uint64_t total_edges = 0;
    for (size_t i = 0; i < n_states; ++i) {
        const uint8_t *a = state_digits + i * COLS;
        uint16_t cnt = 0;
        for (size_t j = 0; j < n_states; ++j) {
            if (i == j) continue;
            const uint8_t *b = state_digits + j * COLS;
            if (valid_transition(a, b)) ++cnt;
        }
        out_cnt[i] = cnt;
        total_edges += cnt;
    }

    /* prefix sums to compute start indices */
    out_start = (uint32_t*)malloc(n_states * sizeof(uint32_t));
    if (!out_start) exit(EXIT_FAILURE);
    uint32_t acc = 0;
    for (size_t i = 0; i < n_states; ++i) {
        out_start[i] = acc;
        acc += out_cnt[i];
    }

    /* allocate destination list */
    out_dest = (uint32_t*)malloc(total_edges * sizeof(uint32_t));
    if (!out_dest) exit(EXIT_FAILURE);

    /* second pass: fill out_dest */
    uint32_t *tmp_pos = (uint32_t*)calloc(n_states, sizeof(uint32_t));
    if (!tmp_pos) exit(EXIT_FAILURE);
    for (size_t i = 0; i < n_states; ++i)
        tmp_pos[i] = out_start[i];

    for (size_t i = 0; i < n_states; ++i) {
        const uint8_t *a = state_digits + i * COLS;
        for (size_t j = 0; j < n_states; ++j) {
            if (i == j) continue;
            const uint8_t *b = state_digits + j * COLS;
            if (valid_transition(a, b)) {
                uint32_t pos = tmp_pos[i]++;
                out_dest[pos] = (uint32_t)j;
            }
        }
    }
    free(tmp_pos);

    /* initialize DP arrays */
    for (size_t i = 0; i < MAX_STATES; ++i) {
        dp_cur[i] = 0;
        dp_next[i] = 0;
    }
    for (size_t i = 0; i < n_states; ++i)
        dp_cur[i] = (seq_t)1;

    initialized = true;
}

/* ------------------------------------------------------------------
 * Main entry point. Computes a(n) for n >= 1.
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range: defined as 0 */
    init();

    /* Extend DP up to the requested n if needed. */
    for (idx_t step = current_n + 1; step <= n; ++step) {
        /* Zero dp_next for the active states */
        for (size_t i = 0; i < n_states; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition */
        for (size_t src = 0; src < n_states; ++src) {
            seq_t ways = dp_cur[src];
            if (ways == (seq_t)0) continue;
            uint16_t outdeg = out_cnt[src];
            if (outdeg == 0) continue;
            uint32_t start = out_start[src];
            for (uint16_t e = 0; e < outdeg; ++e) {
                uint32_t dst = out_dest[start + e];
                dp_next[dst] += ways;
            }
        }

        /* Prepare for next iteration */
        for (size_t i = 0; i < n_states; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }
    current_n = n;

    /* Sum over all ending states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < n_states; ++i)
        total += dp_cur[i];
    return total;
}
