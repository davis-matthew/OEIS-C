/*
 * A234442  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×8 arrays with entries 0..2 such that
 *            every * 2×2 subblock has diagonal sum differing from
 *            antidiagonal sum by 1. * The function computes a(n) by dynamic
 *            programming over rows using a * transfer‑matrix of size 3⁸ =
 *            6561
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 6561          /* 3⁸ */
#define MAX_NEIGH 64             /* upper bound for neighbours per state */

static uint16_t neighbors[STATE_COUNT][MAX_NEIGH];
static uint8_t  neighbor_cnt[STATE_COUNT];
static int     adjacency_built = 0;

/* Build adjacency lists once. */
static void build_adjacency(void) {
    if (adjacency_built) return;
    adjacency_built = 1;

    int pow3[8];
    pow3[0] = 1;
    for (int i = 1; i < 8; ++i) pow3[i] = pow3[i - 1] * 3;

    int prev_digits[8];
    int curr_digits[8];

    for (int prev = 0; prev < STATE_COUNT; ++prev) {
        /* decode previous state */
        int t = prev;
        for (int i = 0; i < 8; ++i) {
            prev_digits[i] = t % 3;
            t /= 3;
        }

        for (int start = 0; start < 3; ++start) {
            curr_digits[0] = start;
            for (int mask = 0; mask < 128; ++mask) {            /* 2^7 possibilities */
                int valid = 1;
                for (int j = 0; j < 7; ++j) {
                    int delta_prev = prev_digits[j + 1] - prev_digits[j];
                    int diff = ( (mask >> j) & 1 ) ? (delta_prev + 1) : (delta_prev - 1);
                    int val = curr_digits[j] + diff;
                    if (val < 0 || val > 2) { valid = 0; break; }
                    curr_digits[j + 1] = val;
                }
                if (!valid) continue;

                /* encode current row to a state index */
                int to = 0;
                for (int i = 0; i < 8; ++i) to += curr_digits[i] * pow3[i];

                /* insert if not already present */
                int found = 0;
                for (int k = 0; k < neighbor_cnt[prev]; ++k)
                    if (neighbors[prev][k] == (uint16_t)to) { found = 1; break; }
                if (!found) {
                    if (neighbor_cnt[prev] < MAX_NEIGH) {
                        neighbors[prev][neighbor_cnt[prev]++] = (uint16_t)to;
                    }
                }
            }
        }
    }
}

/* dynamic storage for results and DP vectors */
static seq_t *results = NULL;      /* results[0] = a(1) etc. */
static size_t result_size = 0;     /* number of computed terms */
static size_t result_cap = 0;
static seq_t *dp = NULL;
static seq_t *dp_next = NULL;
static idx_t current_step = 0;     /* number of rows processed minus 1 */

/* Ensure capacity for an array of seq_t */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? (*cap << 1) : 1;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Extend computation up to n (number of transitions) */
static void compute_up_to(idx_t n) {
    if (n <= current_step) return;
    build_adjacency();

    /* initialise dp with all ones (first row arbitrary) */
    if (!dp) {
        dp = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        dp_next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 1;
        current_step = 0;
        result_size = 0;
        result_cap = 0;
    }

    for (idx_t step = current_step + 1; step <= n; ++step) {
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        /* transition */
        for (int prev = 0; prev < STATE_COUNT; ++prev) {
            seq_t cnt = dp[prev];
            if (cnt == 0) continue;
            for (int k = 0; k < neighbor_cnt[prev]; ++k) {
                uint16_t to = neighbors[prev][k];
                dp_next[to] += cnt;
            }
        }

        /* sum to get a(step) */
        seq_t sum = 0;
        for (int i = 0; i < STATE_COUNT; ++i) sum += dp_next[i];

        /* store result */
        ensure_capacity(result_size + 1, &result_cap, &results);
        results[result_size++] = sum;

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    current_step = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    compute_up_to(n);
    return results[n - 1];
}
