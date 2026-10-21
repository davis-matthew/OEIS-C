/*
 * A202920  Number of nX4 nonnegative integer arrays with each row and column
 *          increasing from zero by 0, 1, 2 or 3.
 * https://oeis.org/A202920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays of nonnegative integers * with
 *            a[1][j] = a[i][1] = 0, * a[i][j] - a[i][j-1] ∈ {0,1,2,3}
 *            (horizontal), * a[i][j] - a[i-1][j] ∈ {0,1,2,3} (vertical)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 44, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation represents each row (except the first, which is fixed)
 *  by its three horizontal differences (d1,d2,d3), each in 0..3.
 *  The transition from one row to the next is allowed exactly when the
 *  cumulative sums of the new differences are not smaller than the
 *  previous ones and do not exceed them by more than 3.
 *
 *  Number of states = 4^3 = 64, so a dynamic‑programming walk on a small
 *  directed graph gives a correct answer in O(n) time and negligible
 *  memory.  All arithmetic is performed in seq_t, which may be an
 *  arbitrary‑precision integer type when built.                */

#include <stdint.h>
#include <stddef.h>

#define STATES 64  /* 4^3 */

static const int initState = 0;   /* (0,0,0) encoded as 0 */

/* Precomputed data for all 64 states */
static uint8_t diff1[STATES];
static uint8_t diff2[STATES];
static uint8_t diff3[STATES];
static uint8_t sum1[STATES];   /* d1                                 */
static uint8_t sum2[STATES];   /* d1 + d2                           */
static uint8_t sum3[STATES];   /* d1 + d2 + d3                      */
static uint8_t adj[STATES][STATES];   /* adjacency list */
static uint8_t adjSize[STATES];

/* one‑time initialization of tables */
static void init_tables(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* decode states */
    for (int s = 0; s < STATES; ++s) {
        diff1[s] = (s >> 4) & 3;
        diff2[s] = (s >> 2) & 3;
        diff3[s] =  s        & 3;
        sum1[s] = diff1[s];
        sum2[s] = diff1[s] + diff2[s];
        sum3[s] = diff1[s] + diff2[s] + diff3[s];
    }

    /* build adjacency */
    for (int s = 0; s < STATES; ++s) {
        adjSize[s] = 0;
        uint8_t R1 = sum1[s];
        uint8_t R2 = sum2[s];
        uint8_t R3 = sum3[s];
        for (int t = 0; t < STATES; ++t) {
            uint8_t S1 = sum1[t];
            uint8_t S2 = sum2[t];
            uint8_t S3 = sum3[t];
            if (   S1 < R1 || S1 > R1 + 3) continue;
            if (   S2 < R2 || S2 > R2 + 3) continue;
            if (   S3 < R3 || S3 > R3 + 3) continue;
            adj[s][adjSize[s]++] = (uint8_t)t;
        }
    }
}

/* Count arrays of size n × 4 with the stated constraints */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    init_tables();

    /* dp[i] = number of ways to reach state i after current step */
    seq_t dp[STATES];
    seq_t newdp[STATES];
    int i;
    for (i = 0; i < STATES; ++i) dp[i] = (seq_t)0;
    dp[initState] = (seq_t)1;      /* first row is fixed (0,0,0) */

    /* perform n-1 transitions (there are n rows in total) */
    idx_t step;
    for (step = 1; step < n; ++step) {
        for (i = 0; i < STATES; ++i) newdp[i] = (seq_t)0;
        for (i = 0; i < STATES; ++i) {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;   /* skip if zero */
            uint8_t sz = adjSize[i];
            const uint8_t *trg = adj[i];
            for (uint8_t k = 0; k < sz; ++k) {
                newdp[trg[k]] += val;
            }
        }
        for (i = 0; i < STATES; ++i) dp[i] = newdp[i];
    }

    /* sum over all final states */
    seq_t total = (seq_t)0;
    for (i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
