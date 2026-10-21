/*
 * A236880  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   sort_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A236880: Count of (n+1)×5 arrays over {0,1,2} such that
   for every 2×2 subblock the sum of max + upper median + lower median
   is the same constant across the array.
   Implementation:
     - Precompute adjacency lists for each possible constant S=0..6.
     - For each S, number of arrays with n subblocks equals the
       number of walks of length n in the directed graph defined by
       these adjacencies.
     - The answer is the sum over all S.
   The function is memoised: subsequent calls with any n reuse
   previously computed DP steps. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sort_four.h"

#define ROWS   243     /* 3^5 possible rows */
#define COLS   5
#define MAX_S   7      /* possible sums 0..6 */

static unsigned char row_vals[ROWS][COLS];
static uint16_t adj[MAX_S][ROWS][ROWS];   /* adjacency lists (full list, unused part ignored) */
static unsigned char adj_len[MAX_S][ROWS];

static int initialized = 0;

/* DP state per S */
static seq_t dp_cur[MAX_S][ROWS];
/* temporary storage for the next step, reused for each S */
static seq_t dp_next[ROWS];

/* memoised totals: totals[S][k] = number of arrays with k subblocks (k = n) and fixed S */
typedef struct {
    seq_t *data;
    size_t len;   /* number of used entries */
    size_t cap;   /* allocated capacity */
} Totals;
static Totals totals[MAX_S] = { {NULL,0,0} };

/* current number of computed steps (subblocks); steps are 0..max_precomputed */
static idx_t max_precomputed = 0;

/* ------------------------------------------------------------------ */

/* initialise static tables and DP state */
static void init(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* generate all rows */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int i = COLS-1; i >= 0; --i) {
            row_vals[r][i] = x % 3;
            x /= 3;
        }
    }

    /* adjacency lists */
    for (int src = 0; src < ROWS; ++src) {
        for (int dst = 0; dst < ROWS; ++dst) {
            unsigned char first_sum = 0;
            int ok = 1;
            for (int j = 0; j < COLS-1; ++j) {
                unsigned char a = row_vals[src][j];
                unsigned char b = row_vals[src][j+1];
                unsigned char c = row_vals[dst][j];
                unsigned char d = row_vals[dst][j+1];
                unsigned char block[4] = {a,b,c,d};
                sort_four(block);
                unsigned char sum = block[1] + block[2] + block[3]; /* lower, upper medians + max */
                if (j == 0)
                    first_sum = sum;
                else if (sum != first_sum) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                unsigned char S = first_sum;
                unsigned char len = adj_len[S][src];
                adj[S][src][len] = (uint16_t)dst;
                adj_len[S][src] = len + 1;
            }
        }
    }

    /* initialise DP state for step 0: one row only (no subblocks yet) */
    for (int S = 0; S < MAX_S; ++S) {
        for (int r = 0; r < ROWS; ++r)
            dp_cur[S][r] = 1;          /* one way to end in each row */
    }

    /* initialise totals[] for step 0 */
    for (int S = 0; S < MAX_S; ++S) {
        totals[S].cap = 1;
        totals[S].data = (seq_t*)malloc(sizeof(seq_t)*1);
        totals[S].len = 1;
        totals[S].data[0] = ROWS;      /* 243 single-row arrays */
    }
    max_precomputed = 0;
}

/* extend memoised tables up to target steps (subblocks) */
static void extend_to(idx_t target)
{
    if (target <= max_precomputed)
        return;

    /* ensure capacity for totals */
    for (int S = 0; S < MAX_S; ++S) {
        if (totals[S].cap <= target) {
            size_t newcap = totals[S].cap ? totals[S].cap * 2 : 1;
            while (newcap <= target)
                newcap *= 2;
            seq_t *tmp = (seq_t*)realloc(totals[S].data, sizeof(seq_t)*newcap);
            totals[S].data = tmp;
            totals[S].cap = newcap;
        }
    }

    while (max_precomputed < target) {
        ++max_precomputed;               /* new step index = max_precomputed */

        for (int S = 0; S < MAX_S; ++S) {
            seq_t *cur = dp_cur[S];

            /* zero dp_next */
            memset(dp_next, 0, sizeof(dp_next));

            for (int src = 0; src < ROWS; ++src) {
                seq_t val = cur[src];
                if (val == 0)
                    continue;
                unsigned char len = adj_len[S][src];
                uint16_t *dests = adj[S][src];
                for (unsigned char k = 0; k < len; ++k) {
                    uint16_t dest = dests[k];
                    dp_next[dest] += val;
                }
            }

            /* sum of all dp_next values = total number of arrays for this S at this step */
            seq_t sum = 0;
            for (int r = 0; r < ROWS; ++r)
                sum += dp_next[r];
            totals[S].data[max_precomputed] = sum;

            /* copy next to cur for next iteration */
            memcpy(cur, dp_next, sizeof(dp_next));
        }
    }
}

seq_t a(idx_t n)
{
    init();
    extend_to(n);          /* n subblocks correspond to n+1 rows */

    seq_t result = 0;
    for (int S = 0; S < MAX_S; ++S)
        result += totals[S].data[n];
    return result;
}
