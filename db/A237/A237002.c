/*
 * A237002  Number of (n+1)X(3+1) 0..3 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A237002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   min4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237002
 * Formula: a(n) = number of (n+1)×4 arrays with values 0..3 such that for every
 * 2×2 subblock the sum of its lower and upper median is the same for all
 * adjacent columns.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/min4.h"

#define ROWS   256          /* 4 columns, values 0..3 */
#define SUMS   7            /* possible sums 0..6 */
#define STATES (ROWS * SUMS)/* 1792 states */

static inline int max4(int a, int b, int c, int d)
{
    int m = a > b ? a : b;
    m = m > c ? m : c;
    m = m > d ? m : d;
    return m;
}

/* for a given pair of rows r1,r2 compute the common sum
   of lower+upper median on the three adjacent 2×2 subblocks.
   Return -1 if the pair is invalid. */
static int8_t pair_sum(uint8_t r1, uint8_t r2)
{
    int sums[3];
    for (int k = 0; k < 3; ++k) {
        int a = (r1 >> (2 * k)) & 3;
        int b = (r1 >> (2 * (k + 1))) & 3;
        int c = (r2 >> (2 * k)) & 3;
        int d = (r2 >> (2 * (k + 1))) & 3;
        int total = a + b + c + d;
        int mn = min4(a, b, c, d);
        int mx = max4(a, b, c, d);
        sums[k] = total - mn - mx;            /* lower + upper median */
    }
    if (sums[0] == sums[1] && sums[1] == sums[2])
        return (int8_t)sums[0];                /* 0..6 */
    return -1;                                 /* invalid */
}

/* ---------- global data for the DP graph ---------- */

static int8_t pair_s[ROWS][ROWS];   /* pair_s[r1][r2] = common sum or -1 */
static size_t *offsets = NULL;       /* CSR offsets, length STATES+1 */
static uint16_t *dest    = NULL;     /* CSR destination rows, length edges_total */
static size_t   edges_total = 0;

/* initialise the graph once */
static void init_graph(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    /* compute pair_s */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            pair_s[r1][r2] = pair_sum((uint8_t)r1, (uint8_t)r2);
        }
    }

    /* count outgoing edges per state */
    size_t per_state[STATES];
    for (size_t i = 0; i < STATES; ++i) per_state[i] = 0;
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int8_t s = pair_s[r1][r2];
            if (s >= 0) {
                size_t state = r1 * SUMS + s;
                per_state[state]++;
            }
        }
    }

    /* allocate CSR structure */
    edges_total = 0;
    offsets = (size_t*)malloc((STATES + 1) * sizeof(size_t));
    if (!offsets) exit(EXIT_FAILURE);
    offsets[0] = 0;
    for (size_t i = 0; i < STATES; ++i) {
        edges_total += per_state[i];
        offsets[i + 1] = edges_total;
    }

    dest = (uint16_t*)malloc(edges_total * sizeof(uint16_t));
    if (!dest) exit(EXIT_FAILURE);

    size_t *ptr = (size_t*)calloc(STATES, sizeof(size_t));
    if (!ptr) exit(EXIT_FAILURE);

    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int8_t s = pair_s[r1][r2];
            if (s >= 0) {
                size_t state = r1 * SUMS + s;
                size_t idx = offsets[state] + ptr[state]++;
                dest[idx] = (uint16_t)r2;   /* only row needed (sum is in state) */
            }
        }
    }
    free(ptr);
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety for indices < 1 */
    init_graph();

    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* initialise dp with counts for a single transition (step 1): */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 0;
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int8_t s = pair_s[r1][r2];
            if (s >= 0) {
                size_t state = r2 * SUMS + s;
                dp[state] += (seq_t)1;
            }
        }
    }
    if (n == 1) {
        seq_t total = 0;
        for (size_t i = 0; i < STATES; ++i) total += dp[i];
        return total;
    }

    /* propagate for steps 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        for (size_t i = 0; i < STATES; ++i) ndp[i] = 0;
        for (size_t state = 0; state < STATES; ++state) {
            seq_t cnt = dp[state];
            if (cnt == 0) continue;
            /* state = old_row * SUMS + sum */
            int s = state % SUMS;
            size_t start = offsets[state];
            size_t end   = offsets[state + 1];
            for (size_t e = start; e < end; ++e) {
                uint16_t r2 = dest[e];
                size_t next_state = (size_t)r2 * SUMS + s;
                ndp[next_state] += cnt;
            }
        }
        for (size_t i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* final sum over all states */
    seq_t result = 0;
    for (size_t i = 0; i < STATES; ++i) result += dp[i];
    return result;
}
