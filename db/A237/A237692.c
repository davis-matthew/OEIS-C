/*
 * A237692  Number of (n+1) X (5+1) 0..3 arrays with the maximum plus the
 *          upper median plus the minimum minus the lower median of every 2 X
 *          2 subblock equal.
 * https://oeis.org/A237692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   idx_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of (n+1)×6 arrays with entries 0..3 for which the
   value   max + upper‑median + min – lower‑median
   is equal for every 2×2 subblock.  The algorithm builds a directed
   graph whose vertices are all 4^6 = 4096 possible rows and whose
   edges connect admissible successive rows for each possible common
   value C∈{0,…,6}.  The number of walks of length n in the graph
   (starting from any row) equals a(n).  All computations are done
   using the build‑time type seq_t.  The routine is memoised so that
   subsequent calls with arbitrary n are answered in O(n) time.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/idx_to_digits.h"

#define ROWS 4096
#define MAX_N 10001   /* valid up to n=10000 */

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} list_t;

static unsigned char allowed[4][4][4][7];
static list_t lists[7][ROWS];
static seq_t curr[7][ROWS];
static seq_t next[7][ROWS];
static seq_t totals[7][MAX_N];
static idx_t computed_n = 0;

/* Push a successor row into the list, extending it if necessary. */
static void push_back(list_t *lst, int val)
{
    if (lst->len == lst->cap) {
        size_t new_cap = lst->cap ? lst->cap << 1 : 8;
        int *tmp = (int *)realloc(lst->data, new_cap * sizeof(int));
        if (!tmp) abort();
        lst->data = tmp;
        lst->cap = new_cap;
    }
    lst->data[lst->len++] = val;
}

/* Recursively construct all successors S that satisfy the constraints
   for a fixed first row R and constant C.  The result is stored in *lst. */
static void dfs_build(int C, const unsigned char *R, unsigned char *S,
                      int j, list_t *lst)
{
    if (j == 5) {  /* all five blocks checked */
        unsigned int row = 0;
        for (int k = 0; k < 6; ++k)
            row = (row << 2) | S[k];
        push_back(lst, (int)row);
        return;
    }
    unsigned char a = R[j];
    unsigned char b = S[j];
    unsigned char c = R[j + 1];
    unsigned char mask = allowed[a][b][c][C];
    for (unsigned char d = 0; d < 4; ++d) {
        if (mask & (1u << d)) {
            S[j + 1] = d;
            dfs_build(C, R, S, j + 1, lst);
        }
    }
}

/* Build all adjacency lists.  Called once. */
static void init_tables(void)
{
    /* 1. pre‑compute masks allowed[ a ][ b ][ c ][ e ] */
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
                for (int d = 0; d < 4; ++d) {
                    int vals[4] = { a, b, c, d };
                    /* sort */
                    for (int i = 0; i < 3; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (vals[i] > vals[j]) {
                                int tmp = vals[i];
                                vals[i] = vals[j];
                                vals[j] = tmp;
                            }
                    int e = vals[3] + vals[2] + vals[0] - vals[1];
                    allowed[a][b][c][e] |= (1u << d);
                }

    /* 2. initialise lists */
    for (int C = 0; C <= 6; ++C)
        for (int r = 0; r < ROWS; ++r) {
            lists[C][r].data = NULL;
            lists[C][r].len = 0;
            lists[C][r].cap = 0;
        }

    /* 3. build adjacency lists */
    unsigned char R[6], S[6];
    for (int r = 0; r < ROWS; ++r) {
        idx_to_digits(R, (uint16_t)r);
        for (int C = 0; C <= 6; ++C) {
            list_t *lst = &lists[C][r];
            for (unsigned char s0 = 0; s0 < 4; ++s0) {
                S[0] = s0;
                dfs_build(C, R, S, 0, lst);
            }
        }
    }

    /* 4. initialise DP for n=0 (single row) */
    for (int C = 0; C <= 6; ++C) {
        for (int r = 0; r < ROWS; ++r)
            curr[C][r] = 1;
        totals[C][0] = (seq_t)ROWS;   /* not used in answer but defined */
    }
    computed_n = 0;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (computed_n == 0 && n >= 1)
        init_tables();

    while (computed_n < n) {
        for (int C = 0; C <= 6; ++C) {
            /* clear next state */
            memset(next[C], 0, sizeof(seq_t) * ROWS);

            /* propagate from current rows */
            for (int r = 0; r < ROWS; ++r) {
                seq_t val = curr[C][r];
                if (!val)
                    continue;
                list_t *lst = &lists[C][r];
                for (size_t i = 0; i < lst->len; ++i)
                    next[C][lst->data[i]] += val;
            }

            /* compute total for this n */
            seq_t sum = 0;
            for (int r = 0; r < ROWS; ++r)
                sum += next[C][r];
            totals[C][computed_n + 1] = sum;

            /* prepare for next iteration */
            memcpy(curr[C], next[C], sizeof(seq_t) * ROWS);
        }
        ++computed_n;
    }

    /* sum over all possible common constants C */
    seq_t result = 0;
    for (int C = 0; C <= 6; ++C)
        result += totals[C][n];
    return result;
}
