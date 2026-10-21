/*
 * A236882  Number of (n+1)X(6+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays with entries 0..2 such that for
 *            every 2×2 subblock the sum of the largest, the upper median, and
 *            the lower median is equal (consequently, the sum of the three
 *            largest values of each block is a constant)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STATE_COUNT 2187           /* 3^7 possible rows */
#define MAX_CONST 7               /* possible constants 0..6 */

/* Storage for the 7‑digit base‑3 representation of each state (row). */
static unsigned char row_val[STATE_COUNT][7];

/* Dynamic adjacency lists for each constant and each state. */
struct RowAdj {
    unsigned short *neigh;  /* array of neighbor state indices      */
    unsigned short  count;  /* number of stored neighbors           */
    unsigned short  capacity; /* allocated capacity */
};
static struct RowAdj adj[MAX_CONST][STATE_COUNT];

/* Flags to build tables once. */
static bool tables_built = false;

/* Utility: push a neighbor into the adjacency list. */
static void push_neighbor(int c, unsigned short from, unsigned short to)
{
    struct RowAdj *ra = &adj[c][from];
    if (ra->count == ra->capacity) {
        unsigned short new_cap = ra->capacity == 0 ? 4 : ra->capacity * 2;
        unsigned short *tmp = (unsigned short*)realloc(ra->neigh, new_cap * sizeof(unsigned short));
        if (!tmp) exit(1);
        ra->neigh = tmp;
        ra->capacity = new_cap;
    }
    ra->neigh[ra->count++] = to;
}

/* Pre‑compute states and adjacency lists. */
static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* Build the representation of each state. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int v = s;
        for (int pos = 0; pos < 7; ++pos) {
            row_val[s][pos] = (unsigned char)(v % 3);
            v /= 3;
        }
    }

    /* Scan all ordered pairs of rows to determine admissible transitions. */
    for (int a = 0; a < STATE_COUNT; ++a) {
        const unsigned char *A = row_val[a];
        for (int b = 0; b < STATE_COUNT; ++b) {
            const unsigned char *B = row_val[b];
            int c = -1;
            bool ok = true;
            for (int j = 0; j <= 5; ++j) {
                int a0 = A[j], a1 = A[j + 1];
                int b0 = B[j], b1 = B[j + 1];
                int sum4 = a0 + a1 + b0 + b1;
                int m = a0;
                if (b0 < m) m = b0;
                if (a1 < m) m = a1;
                if (b1 < m) m = b1;
                int cur = sum4 - m;   /* sum of largest three values */
                if (c == -1)
                    c = cur;
                else if (cur != c) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                /* Store directed edge a -> b for constant c. */
                push_neighbor(c, (unsigned short)a, (unsigned short)b);
            }
        }
    }
}

/* allocate DP arrays once */
static seq_t *dp = NULL;
static seq_t *newdp = NULL;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* offset is 1, but just in case */
        return (seq_t)0;
    }
    build_tables();

    /* Allocate DP arrays on first call. */
    if (!dp) {
        dp = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
        newdp = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
    }

    seq_t result = (seq_t)0;
    /* For each possible constant, count arrays that use this constant. */
    for (int c = 0; c < MAX_CONST; ++c) {
        /* Initialize dp with 1 for every first row. */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)1;

        /* Perform DP for n steps (n = target number of 2×2 blocks vertically). */
        for (idx_t step = 0; step < n; ++step) {
            /* Zero newdp. */
            for (int i = 0; i < STATE_COUNT; ++i) newdp[i] = (seq_t)0;

            /* Transition step. */
            for (int u = 0; u < STATE_COUNT; ++u) {
                seq_t val = dp[u];
                if (val == (seq_t)0) continue;
                const struct RowAdj *ra = &adj[c][u];
                for (unsigned short k = 0; k < ra->count; ++k) {
                    unsigned short v = ra->neigh[k];
                    newdp[v] = newdp[v] + val;
                }
            }
            /* Swap dp and newdp for next iteration. */
            seq_t *tmp = dp;
            dp = newdp;
            newdp = tmp;
        }

        /* Sum counts for all ending rows. */
        seq_t countC = (seq_t)0;
        for (int i = 0; i < STATE_COUNT; ++i) countC = countC + dp[i];

        result = result + countC;
    }
    return result;
}
