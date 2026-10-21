/*
 * A233922  Number of (n+1)X(2+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 8
 *          and no adjacent elements equal.
 * https://oeis.org/A233922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes a(n) for A233922 by dynamic programming over all
   3‑cell row states.  The first row may be any state; each transition
   to the next row is allowed iff vertical neighbours differ and all
   2×2 subblocks with the previous row satisfy
     |a−b|+|b−d|+|d−c|+|c−a| = 8.
   The function works for any build’s seq_t and idx_t types. */

#include <stdint.h>

#define NUM_STATES 80
#define MASK_SIZE 2   /* 2 uint64_t give 128 bits > 80 */

/* encode all 3‑cell rows a,b,c (0..4) with a!=b && b!=c; store 80 states */
static int state_val[NUM_STATES][3];
/* compatibility bit masks: for each state i, compat_mask[i][k] has 1
   bit j if state i can be followed by state j */
static unsigned long long compat_mask[NUM_STATES][MASK_SIZE];

/* initialise state list and compatibility masks once */
static void init_states(void) {
    static int initialized = 0;
    if (initialized) return;
    int idx = 0;
    for (int a = 0; a < 5; ++a)
        for (int b = 0; b < 5; ++b)
            if (a != b)
                for (int c = 0; c < 5; ++c)
                    if (b != c) {
                        state_val[idx][0] = a;
                        state_val[idx][1] = b;
                        state_val[idx][2] = c;
                        ++idx;
                    }

    /* build compatibility masks */
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = 0; j < NUM_STATES; ++j) {
            int *pre = state_val[i];
            int *cur = state_val[j];

            /* vertical adjacency must differ */
            if (pre[0] == cur[0] || pre[1] == cur[1] || pre[2] == cur[2]) continue;

            /* left 2×2 block */
            int a = pre[0], b = pre[1], c1 = cur[0], d = cur[1];
            int s = (a > b ? a - b : b - a) +
                    (b > d ? b - d : d - b) +
                    (d > c1 ? d - c1 : c1 - d) +
                    (c1 > a ? c1 - a : a - c1);
            if (s != 8) continue;

            /* right 2×2 block */
            a = pre[1]; b = pre[2]; c1 = cur[1]; d = cur[2];
            s = (a > b ? a - b : b - a) +
                (b > d ? b - d : d - b) +
                (d > c1 ? d - c1 : c1 - d) +
                (c1 > a ? c1 - a : a - c1);
            if (s != 8) continue;

            /* if all tests passed, set bit j */
            compat_mask[i][j >> 6] |= 1ULL << (j & 63);
        }
    }
    initialized = 1;
}

/* main sequence function */
seq_t a(idx_t n) {
    init_states();

    /* dynamic programming arrays: counts for each row state */
    seq_t dp_prev[NUM_STATES], dp_cur[NUM_STATES];
    for (int i = 0; i < NUM_STATES; ++i) dp_prev[i] = (seq_t)1;

    /* we have n+1 rows → n transitions */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = (seq_t)0;
        for (int i = 0; i < NUM_STATES; ++i) {
            seq_t val = dp_prev[i];
            if (!val) continue;
            /* iterate over all possible next states j */
            for (int j = 0; j < NUM_STATES; ++j) {
                if (compat_mask[i][j >> 6] & (1ULL << (j & 63))) {
                    dp_cur[j] += val;
                }
            }
        }
        /* copy current to previous for next step */
        for (int i = 0; i < NUM_STATES; ++i) dp_prev[i] = dp_cur[i];
    }

    /* sum all counts for the last row states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_STATES; ++i) total += dp_prev[i];
    return total;
}
