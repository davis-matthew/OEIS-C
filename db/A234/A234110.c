/*
 * A234110  Number of (n+1)X(4+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 12.
 * https://oeis.org/A234110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over {0,1,2,3,4} such that every
 *            2×2 subblock has the sum of the absolute differences of all six
 *            edges and diagonals equal to 12. The function returns the count
 *            as a seq_t. It uses a linear‑time DP over the rows with a
 *            pre‑computed adjacency table
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   iabs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/iabs.h"

/* Number of 5‑digit rows in base‑5 (5^5 = 3125). */
#define M 3125

/* Pre‑computed 5‑digit representation of each state. */
static unsigned char digits[M][5];

/* Adjacency lists: succ[prev] holds rows that can follow 'prev'. */
static int **succ;
static int *succCount;

/* DP arrays (one for previous row counts, one for current). */
static seq_t dpPrev[M];
static seq_t dpCurr[M];

static int initialized = 0;

/* Pre‑compute the base‑5 digits for all rows. */
static void precompute_digits(void) {
    for (int s = 0; s < M; ++s) {
        int t = s;
        for (int c = 0; c < 5; ++c) {
            digits[s][c] = (unsigned char)(t % 5);
            t /= 5;
        }
    }
}

/* Check whether two rows are compatible as adjacent rows in the grid. */
static inline int compatible(int r1, int r2) {
    const unsigned char *a = digits[r1];
    const unsigned char *b = digits[r2];
    for (int c = 0; c < 4; ++c) {
        int a0 = a[c], a1 = a[c + 1];
        int b0 = b[c], b1 = b[c + 1];
        int sum = iabs(a0 - a1) + iabs(b0 - b1) +    /* horizontal edges */
                  iabs(a0 - b0) + iabs(a1 - b1) +    /* vertical edges */
                  iabs(a0 - b1) + iabs(a1 - b0);     /* diagonals */
        if (sum != 12) return 0;
    }
    return 1;
}

/* Initialise adjacency lists once. */
static void init(void) {
    if (initialized) return;

    precompute_digits();

    succCount = (int *)malloc(M * sizeof(int));
    succ      = (int **)malloc(M * sizeof(int *));
    memset(succCount, 0, M * sizeof(int));

    /* First pass: count successors for each row. */
    for (int prev = 0; prev < M; ++prev) {
        int cnt = 0;
        for (int next = 0; next < M; ++next)
            if (compatible(prev, next)) ++cnt;
        succCount[prev] = cnt;
    }

    /* Allocate and fill adjacency lists. */
    for (int prev = 0; prev < M; ++prev) {
        succ[prev] = (int *)malloc(succCount[prev] * sizeof(int));
        int idx = 0;
        for (int next = 0; next < M; ++next)
            if (compatible(prev, next)) succ[prev][idx++] = next;
    }

    initialized = 1;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */

    init();

    /* Start with one possibility for the first row. */
    for (int i = 0; i < M; ++i) dpPrev[i] = (seq_t)1;

    /* Each step adds a new row. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < M; ++i) dpCurr[i] = (seq_t)0;

        for (int prev = 0; prev < M; ++prev) {
            seq_t val = dpPrev[prev];
            if (val == (seq_t)0) continue;
            int *s = succ[prev];
            int sz = succCount[prev];
            for (int k = 0; k < sz; ++k) {
                int next = s[k];
                dpCurr[next] += val;
            }
        }

        /* Move to next step. */
        for (int i = 0; i < M; ++i) dpPrev[i] = dpCurr[i];
    }

    /* Sum counts for all possible bottom rows. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < M; ++i) total += dpPrev[i];
    return total;
}
