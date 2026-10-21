/*
 * A237900  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3} satisfying the
 *            neighbour 2×2‑block constraint described in A237900
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for the helper functions and standard types. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Constant parameters chosen to cover all possibilities of a 2×2 block. */
#define ROWS   64          /* 4^3 possible rows written in base‑4 */
#define FMAX   6           /* maximum value of f for a 2×2 block */
#define STATES (ROWS*7*7)  /* state = (row, f(prev0), f(prev1)) */

/* Decode a row index into its three cell values (each 0..3). */
static uint8_t row_val[ROWS][3];

/* Precomputed f value for every 4‑tuple of cells (seen as 8‑bit mask). */
static uint8_t block_val[256];

/* For each pair of rows, the f value of the left and right 2×2 blocks. */
static uint8_t pairF[ROWS][ROWS][2];

/* Horizontal compatibility: 1 if f(left) and f(right) differ by 1. */
static uint8_t horiz_ok[ROWS][ROWS];

/* Transition tables: for each state, list of next states. */
static int  nextIdx[STATES][64];
static uint8_t nextCount[STATES];

/* DP arrays. They are allocated exactly once. */
static seq_t *base_counts = NULL;
static seq_t *cur = NULL;
static seq_t *nxt = NULL;

static bool initialized = false;

/* Helper to compute a unique integer for a state. */
static inline int state_index(int row, int f0, int f1)
{
    return ((row * 7) + f0) * 7 + f1;
}

/* Pre‑computations. */
static void precompute(void)
{
    /* Decode all row indices. */
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < 3; ++c)
            row_val[r][c] = (r >> (c * 2)) & 3;
    }

    /* Compute f for every 4‑tuple (0..3 values). */
    for (int idx = 0; idx < 256; ++idx)
    {
        uint8_t a0 = idx & 3;
        uint8_t a1 = (idx >> 2) & 3;
        uint8_t a2 = (idx >> 4) & 3;
        uint8_t a3 = (idx >> 6) & 3;
        uint8_t arr[4] = { a0, a1, a2, a3 };
        /* simple bubble sort */
        for (int i = 0; i < 4; ++i)
            for (int j = i + 1; j < 4; ++j)
                if (arr[i] > arr[j])
                {
                    uint8_t t = arr[i];
                    arr[i] = arr[j];
                    arr[j] = t;
                }
        block_val[idx] = arr[3] + arr[2] - arr[0];
    }

    /* Precompute f for left/right blocks of each row pair and horizontal
       compatibility. */
    for (int r1 = 0; r1 < ROWS; ++r1)
    {
        for (int r2 = 0; r2 < ROWS; ++r2)
        {
            uint8_t fL = block_val[
                (row_val[r1][0]       )
                | (row_val[r1][1] << 2)
                | (row_val[r2][0] << 4)
                | (row_val[r2][1] << 6)];

            uint8_t fR = block_val[
                (row_val[r1][1]       )
                | (row_val[r1][2] << 2)
                | (row_val[r2][1] << 4)
                | (row_val[r2][2] << 6)];

            pairF[r1][r2][0] = fL;
            pairF[r1][r2][1] = fR;
            horiz_ok[r1][r2] = (abs((int)fL - (int)fR) == 1) ? 1 : 0;
        }
    }

    /* Build transition lists for every state. */
    for (int r_cur = 0; r_cur < ROWS; ++r_cur)
    {
        for (int f_prev0 = 0; f_prev0 <= FMAX; ++f_prev0)
        {
            for (int f_prev1 = 0; f_prev1 <= FMAX; ++f_prev1)
            {
                int s = state_index(r_cur, f_prev0, f_prev1);
                nextCount[s] = 0;
                for (int r_next = 0; r_next < ROWS; ++r_next)
                {
                    if (!horiz_ok[r_cur][r_next]) continue;
                    uint8_t f0 = pairF[r_cur][r_next][0];
                    uint8_t f1 = pairF[r_cur][r_next][1];
                    if (abs((int)f_prev0 - (int)f0) == 1 &&
                        abs((int)f_prev1 - (int)f1) == 1)
                    {
                        int t = state_index(r_next, f0, f1);
                        nextIdx[s][nextCount[s]++] = t;
                    }
                }
            }
        }
    }

    /* Allocate DP arrays. */
    base_counts = (seq_t*)malloc(sizeof(seq_t) * STATES);
    cur         = (seq_t*)malloc(sizeof(seq_t) * STATES);
    nxt         = (seq_t*)malloc(sizeof(seq_t) * STATES);
    for (int i = 0; i < STATES; ++i) base_counts[i] = 0;

    /* Initialise base_counts for n == 1 (two rows). */
    for (int r0 = 0; r0 < ROWS; ++r0)
    {
        for (int r1 = 0; r1 < ROWS; ++r1)
        {
            if (!horiz_ok[r0][r1]) continue;
            uint8_t f0 = pairF[r0][r1][0];
            uint8_t f1 = pairF[r0][r1][1];
            int s = state_index(r1, f0, f1);
            base_counts[s] += 1;
        }
    }
}

/* Main entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized)
    {
        precompute();
        initialized = true;
    }

    /* Copy base_counts into the current DP layer. */
    for (int i = 0; i < STATES; ++i) cur[i] = base_counts[i];

    if (n == 1)
    {
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i) total += cur[i];
        return total;
    }

    /* Run the DP for columns 3..n+1. */
    for (idx_t step = 2; step <= n; ++step)
    {
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        for (int s = 0; s < STATES; ++s)
        {
            seq_t cnt = cur[s];
            if (!cnt) continue;
            uint8_t len = nextCount[s];
            for (int k = 0; k < len; ++k)
            {
                int t = nextIdx[s][k];
                nxt[t] += cnt;
            }
        }

        /* Swap arrays. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum the final counts. */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) result += cur[i];
    return result;
}
