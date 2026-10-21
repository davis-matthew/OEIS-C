/*
 * A233642  Number of (n+1)X(6+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 2.
 * https://oeis.org/A233642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays over {0,1,2} whose every 2×2
 *            subblock has exactly two neighboring edges of difference ±1 and
 *            the other two edges of difference 0. The value is obtained by
 *            dynamic programming over rows using the transfer adjacency
 *            matrix of all 7‑cell rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_STATES 2187   /* 3^7 */

static signed char state_digits[NUM_STATES][7];
static int *adj_offset = NULL;      /* size NUM_STATES+1 */
static int16_t *adj_neighbors = NULL;  /* total number of edges */
static bool initialized = false;

/* Check whether two 7‑cell rows can be consecutive. */
static bool compatible(int a, int b)
{
    for (int col = 0; col < 6; ++col) {
        int top_left   = state_digits[a][col];
        int top_right  = state_digits[a][col + 1];
        int bot_left   = state_digits[b][col];
        int bot_right  = state_digits[b][col + 1];

        int h1 = top_right - top_left;   /* horizontal, top row   */
        int h2 = bot_right  - bot_left;  /* horizontal, bottom row*/
        int v1 = bot_left   - top_left;  /* vertical,  left column*/
        int v2 = bot_right  - top_right; /* vertical,  right column*/

        if (h1 < -1 || h1 > 1 ||
            h2 < -1 || h2 > 1 ||
            v1 < -1 || v1 > 1 ||
            v2 < -1 || v2 > 1) return false;

        int sumsq = h1 * h1 + h2 * h2 + v1 * v1 + v2 * v2;
        if (sumsq != 2) return false;
    }
    return true;
}

/* Build adjacency lists for the transfer matrix. */
static void init_matrices(void)
{
    /* Pre‑compute base‑3 digits for all rows. */
    for (int s = 0; s < NUM_STATES; ++s) {
        int t = s;
        for (int col = 6; col >= 0; --col) {
            state_digits[s][col] = t % 3;
            t /= 3;
        }
    }

    /* Count neighbours for each state. */
    int *cnt = (int*)calloc(NUM_STATES, sizeof(int));
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = i; j < NUM_STATES; ++j) {
            if (compatible(i, j)) {
                ++cnt[i];
                if (i != j) ++cnt[j];
            }
        }
    }

    /* Offset array for adjacency list. */
    adj_offset = (int*)malloc((NUM_STATES + 1) * sizeof(int));
    int total = 0;
    adj_offset[0] = 0;
    for (int i = 0; i < NUM_STATES; ++i) {
        total += cnt[i];
        adj_offset[i + 1] = total;
    }

    /* Allocate neighbour array. */
    adj_neighbors = (int16_t*)malloc(total * sizeof(int16_t));
    int *curr = (int*)malloc(NUM_STATES * sizeof(int));
    for (int i = 0; i < NUM_STATES; ++i) curr[i] = adj_offset[i];

    /* Fill adjacency lists. */
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = i; j < NUM_STATES; ++j) {
            if (compatible(i, j)) {
                adj_neighbors[curr[i]++] = (int16_t)j;
                if (i != j) {
                    adj_neighbors[curr[j]++] = (int16_t)i;
                }
            }
        }
    }

    free(curr);
    free(cnt);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_matrices();

    static seq_t buf0[NUM_STATES];
    static seq_t buf1[NUM_STATES];
    seq_t *cur = buf0;
    seq_t *next = buf1;

    /* First row can be any of the 2187 possibilities. */
    for (int i = 0; i < NUM_STATES; ++i) {
        cur[i] = (seq_t)1;
        next[i] = (seq_t)0;
    }

    for (idx_t step = 1; step <= n; ++step) {
        /* Zero the next vector. */
        for (int i = 0; i < NUM_STATES; ++i) next[i] = (seq_t)0;

        /* Propagate counts through allowed transitions. */
        for (int i = 0; i < NUM_STATES; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            int start = adj_offset[i];
            int end = adj_offset[i + 1];
            for (int k = start; k < end; ++k) {
                int j = adj_neighbors[k];
                next[j] += val;
            }
        }

        /* Swap current and next vectors. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all last rows. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < NUM_STATES; ++i) result += cur[i];
    return result;
}
