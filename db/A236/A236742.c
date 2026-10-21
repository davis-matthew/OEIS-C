/*
 * A236742  Number of (n+1)X(4+1) 0..2 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A236742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over S=0..4 of 1ᵀ·A_Sⁿ·1, where A_S is the *
 *            adjacency matrix of 243×243 rows over {0,1} with rows r₁,r₂ *
 *            allowed if the upper‑median plus lower‑median of each 2×2
 *            subblock * is equal and equal to S. * The function uses dynamic
 *            programming: it iterates n times, * multiplying the curre...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   median_sum4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/median_sum4.h"

#define ROWS 243      /* 3^5 possible row patterns */
#define SNS   5       /* number of possible median sums */

/* --------------------------------------------------------------------- */
/*  Global data structures (initialized once)                            */
/* --------------------------------------------------------------------- */
static unsigned char rows[ROWS][5];        /* base‑3 digits of each row */
static int deg[SNS][ROWS];                /* number of neighbours per row, per S */
static int **adj[SNS];                    /* adj[ S ][ i ] -> array of j's */

/* --------------------------------------------------------------------- */
/*  Helper: 0..243-1 rows recognized as base‑3 numbers (least significant digit first)  */
/* --------------------------------------------------------------------- */
static void gen_rows(void)
{
    for (size_t i = 0; i < ROWS; ++i) {
        size_t x = i;
        for (int c = 0; c < 5; ++c) {
            rows[i][c] = (unsigned char)(x % 3);
            x /= 3;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Compute the median sum of a 2×2 block defined by four entries           */

/* --------------------------------------------------------------------- */
/*  Build adjacency lists: for each S and row i, list of rows j that can follow it   */
/* --------------------------------------------------------------------- */
static void init_adjacency(void)
{
    gen_rows();

    /* 1. count degrees */
    for (int S = 0; S < SNS; ++S) {
        for (int i = 0; i < ROWS; ++i) deg[S][i] = 0;
    }

    for (int S = 0; S < SNS; ++S) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                int s0 = median_sum4(rows[i][0], rows[i][1],
                                     rows[j][0], rows[j][1]);
                int s1 = median_sum4(rows[i][1], rows[i][2],
                                     rows[j][1], rows[j][2]);
                int s2 = median_sum4(rows[i][2], rows[i][3],
                                     rows[j][2], rows[j][3]);
                int s3 = median_sum4(rows[i][3], rows[i][4],
                                     rows[j][3], rows[j][4]);
                if (s0 == s1 && s1 == s2 && s2 == s3 && s0 == S) {
                    deg[S][i]++;
                }
            }
        }
    }

    /* 2. allocate adjacency lists */
    for (int S = 0; S < SNS; ++S) {
        adj[S] = (int **)malloc(ROWS * sizeof(int *));
        for (int i = 0; i < ROWS; ++i) {
            if (deg[S][i] > 0)
                adj[S][i] = (int *)malloc(deg[S][i] * sizeof(int));
            else
                adj[S][i] = NULL;
        }
    }

    /* 3. fill adjacency lists */
    for (int S = 0; S < SNS; ++S) {
        for (int i = 0; i < ROWS; ++i) {
            int *ptr = adj[S][i];
            for (int j = 0; j < ROWS; ++j) {
                int s0 = median_sum4(rows[i][0], rows[i][1],
                                     rows[j][0], rows[j][1]);
                int s1 = median_sum4(rows[i][1], rows[i][2],
                                     rows[j][1], rows[j][2]);
                int s2 = median_sum4(rows[i][2], rows[i][3],
                                     rows[j][2], rows[j][3]);
                int s3 = median_sum4(rows[i][3], rows[i][4],
                                     rows[j][3], rows[j][4]);
                if (s0 == s1 && s1 == s2 && s2 == s3 && s0 == S) {
                    *ptr++ = j;
                }
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Static buffers for dynamic programming                               */
/* --------------------------------------------------------------------- */
static int initialized = 0;
static seq_t array1[SNS][ROWS];
static seq_t array2[SNS][ROWS];
static seq_t (*cur)[ROWS] = array1;
static seq_t (*nxt)[ROWS] = array2;

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_adjacency();
        initialized = 1;
    }

    /* initialise current vector to 1 for every row, every S */
    for (int S = 0; S < SNS; ++S)
        for (int i = 0; i < ROWS; ++i)
            cur[S][i] = 1;

    /* iterate n times (each step adds one row) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero the next vector */
        for (int S = 0; S < SNS; ++S)
            for (int i = 0; i < ROWS; ++i)
                nxt[S][i] = 0;

        /* multiply by adjacency lists */
        for (int S = 0; S < SNS; ++S) {
            for (int i = 0; i < ROWS; ++i) {
                seq_t val = cur[S][i];
                if (val == 0) continue;
                int *list = adj[S][i];
                int d = deg[S][i];
                for (int k = 0; k < d; ++k) {
                    int j = list[k];
                    nxt[S][j] += val;
                }
            }
        }

        /* swap buffers */
        seq_t (*tmp)[ROWS] = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all entries of the final vector over all S */
    seq_t total = 0;
    for (int S = 0; S < SNS; ++S)
        for (int i = 0; i < ROWS; ++i)
            total += cur[S][i];

    return total;
}
