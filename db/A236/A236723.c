/*
 * A236723  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A236723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS A236723: 
    number of (n+1)×4 arrays over {0,1,2} whose every 2×2 subblock satisfies
    max + upper median + lower median – min  = constant.
    The algorithm builds, for each possible constant C = sum – 2·min,
    a directed graph of the 81 possible 4‑digit rows.  Each edge
    (i→j) represents a pair of consecutive rows that makes all three
    2×2 subblocks equal to C.  The number of admissible arrays of
    length (n+1) rows equals the sum over C of the number of walks
    of length n in that graph.  Dynamic programming over the
    adjacency lists yields an O(n + 𝐸) time solution, with the
    constant 𝐸 = total number of such edges (≈550). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATES 81          /* 3^4 possible rows */
#define MAXC    9          /* possible constants C = sum – 2*min (0..8) */

/* Pre‑computed digits of each row (base‑3 representation). */
static int state_digits[STATES][4];

/* For each constant C the adjacency list of each state. */
static int deg[MAXC][STATES];
static int edges[MAXC][STATES][STATES];   /* maximum possible neighbours */

/* Dynamic programming tables: counts of sequences ending in state i
   with constant C after some number of steps. */
static seq_t dp_cur[MAXC][STATES];
static seq_t dp_next[MAXC][STATES];

/* Memoised results of a(n).  res[0] is unused. */
static seq_t *res = NULL;
static size_t res_cap = 0;
static idx_t max_computed = 0;   /* largest n for which res[n] exists */

/* Helper to initialise data structures once. */
static void init(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Compute digits of each row. */
    for (int s = 0; s < STATES; ++s) {
        int t = s;
        for (int k = 0; k < 4; ++k) {
            state_digits[s][k] = t % 3;
            t /= 3;
        }
    }

    /* Build adjacency lists for each constant C. */
    for (int i = 0; i < STATES; ++i) {
        for (int j = 0; j < STATES; ++j) {
            int v[3];
            for (int col = 0; col < 3; ++col) {
                int a = state_digits[i][col];
                int b = state_digits[i][col+1];
                int c = state_digits[j][col];
                int d = state_digits[j][col+1];
                int sum = a + b + c + d;
                int m = a;
                if (b < m) m = b;
                if (c < m) m = c;
                if (d < m) m = d;
                v[col] = sum - 2*m;          /* constant C for this subblock */
            }
            if (v[0] == v[1] && v[1] == v[2]) {
                int C = v[0];
                int d0 = deg[C][i]++;
                edges[C][i][d0] = j;        /* i → j */
            }
        }
    }

    /* Initially, sequences of length 1 (no adjacency).  Start with count 1
       in every state for every constant. */
    for (int C = 0; C < MAXC; ++C)
        for (int i = 0; i < STATES; ++i)
            dp_cur[C][i] = 1;

    /* Allocate the result table. */
    res_cap = 1;
    res = (seq_t *)malloc(res_cap * sizeof(seq_t));
    res[0] = 0;          /* unused; a(0) is not defined */
    max_computed = 0;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    init();

    if (n > max_computed) {
        /* Ensure result array large enough. */
        if ((size_t)(n + 1) > res_cap) {
            seq_t *tmp = (seq_t *)realloc(res, (n + 1) * sizeof(seq_t));
            if (!tmp) abort();          /* fatal memory error */
            res = tmp;
            res_cap = n + 1;
        }

        /* Extend DP from max_computed+1 up to n. */
        for (idx_t step = max_computed + 1; step <= n; ++step) {
            /* Zero dp_next. */
            for (int C = 0; C < MAXC; ++C)
                memset(dp_next[C], 0, STATES * sizeof(seq_t));

            /* Propagate one step for each constant. */
            for (int C = 0; C < MAXC; ++C) {
                for (int i = 0; i < STATES; ++i) {
                    seq_t val = dp_cur[C][i];
                    if (!val) continue;
                    int dcnt = deg[C][i];
                    for (int e = 0; e < dcnt; ++e) {
                        int j = edges[C][i][e];
                        dp_next[C][j] += val;
                    }
                }
            }

            /* Sum all counts to get a(step). */
            seq_t total = 0;
            for (int C = 0; C < MAXC; ++C)
                for (int i = 0; i < STATES; ++i)
                    total += dp_next[C][i];
            res[step] = total;

            /* Copy dp_next into dp_cur for next iteration. */
            for (int C = 0; C < MAXC; ++C)
                memcpy(dp_cur[C], dp_next[C], STATES * sizeof(seq_t));
        }

        max_computed = n;
    }
    return res[n];
}
