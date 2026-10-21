/*
 * A234213  Number of (n+1)X(4+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 11.
 * https://oeis.org/A234213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A234213: number of (n+1)×5 arrays over 0..4 with every 2×2
 * subblock satisfying the given sum-of-squares condition.
 *
 * Algorithm: treat each 5‑column row as a state (there are 5^5 = 3125).
 * Two rows are compatible iff all four 2×2 blocks between them
 * have the required sum of squares (= 11).
 * The problem reduces to counting length‑(n+1) paths in the directed
 * graph of compatible rows.  Dynamic programming over the row states
 * yields a linear‑time (in n) algorithm with a precomputation of the
 * adjacency lists.  Results are memoised so successive calls are fast.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_STATES 3125ULL   /* 5^5 */

static bool adjacency_init = false;
static uint8_t row_vals[NUM_STATES][5];

static size_t *adj_deg = NULL;
static idx_t **adj_list = NULL;

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *ans = NULL;
static idx_t cur_max_n = 0;

/* pre‑compute the digit vectors for all states */
static void compute_row_vals(void)
{
    for (idx_t s = 0; s < NUM_STATES; ++s) {
        idx_t tmp = s;
        for (int i = 0; i < 5; ++i) {
            row_vals[s][i] = (uint8_t)(tmp % 5);
            tmp /= 5;
        }
    }
}

/* test whether two rows satisfy the 2×2 condition */
static bool rows_compatible(idx_t a, idx_t b)
{
    const uint8_t *A = row_vals[a];
    const uint8_t *B = row_vals[b];
    for (int c = 0; c < 4; ++c) {
        int a0 = A[c], a1 = A[c + 1];
        int b0 = B[c], b1 = B[c + 1];
        int s = 0, d;
        d = a0 - a1; s += d * d;
        d = a0 - b0; s += d * d;
        d = a1 - b1; s += d * d;
        d = b0 - b1; s += d * d;
        d = a0 - b1; s += d * d;
        d = a1 - b0; s += d * d;
        if (s != 11)
            return false;
    }
    return true;
}

/* build adjacency lists */
static void init_adjacency(void)
{
    compute_row_vals();

    adj_deg = (size_t*)calloc(NUM_STATES, sizeof(size_t));
    for (idx_t s = 0; s < NUM_STATES; ++s)
        for (idx_t t = 0; t < NUM_STATES; ++t)
            if (rows_compatible(s, t))
                ++adj_deg[s];

    adj_list = (idx_t**)malloc(NUM_STATES * sizeof(idx_t*));
    for (idx_t s = 0; s < NUM_STATES; ++s) {
        if (adj_deg[s] == 0) {
            adj_list[s] = NULL;
            continue;
        }
        adj_list[s] = (idx_t*)malloc(adj_deg[s] * sizeof(idx_t));
        size_t pos = 0;
        for (idx_t t = 0; t < NUM_STATES; ++t)
            if (rows_compatible(s, t))
                adj_list[s][pos++] = t;
    }

    dp_cur  = (seq_t*)calloc(NUM_STATES, sizeof(seq_t));
    dp_next = (seq_t*)calloc(NUM_STATES, sizeof(seq_t));

    for (idx_t i = 0; i < NUM_STATES; ++i)
        dp_cur[i] = (seq_t)1;  /* one path ending in state i of length 1 */

    adjacency_init = true;
}

/* ensure answer array computed up to n */
static void ensure_data_upto(idx_t n)
{
    if (!adjacency_init)
        init_adjacency();

    if (ans == NULL) {
        ans = (seq_t*)calloc(1, sizeof(seq_t));  /* index 0 unused */
        cur_max_n = 0;
    }

    if (n > cur_max_n) {
        ans = (seq_t*)realloc(ans, (n + 1) * sizeof(seq_t));
        for (idx_t i = cur_max_n + 1; i <= n; ++i)
            ans[i] = (seq_t)0;

        /* compute DP from cur_max_n+1 up to n */
        for (idx_t step = cur_max_n + 1; step <= n; ++step) {
            memset(dp_next, 0, NUM_STATES * sizeof(seq_t));

            for (idx_t s = 0; s < NUM_STATES; ++s) {
                if (dp_cur[s] == (seq_t)0)
                    continue;
                seq_t val = dp_cur[s];
                size_t d = adj_deg[s];
                for (size_t p = 0; p < d; ++p) {
                    idx_t t = adj_list[s][p];
                    dp_next[t] += val;
                }
            }

            seq_t sum = (seq_t)0;
            for (idx_t t = 0; t < NUM_STATES; ++t)
                sum += dp_next[t];
            ans[step] = sum;

            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        cur_max_n = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined, but safe */
    ensure_data_upto(n);
    return ans[n];
}
