/*
 * A234172  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   abs_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A234172: Number of (n+1)×4 arrays with entries 0…6
 * such that every 2×2 subblock has its main‑diagonal sum
 * differing from its anti‑diagonal sum by exactly 1.
 *
 * Computation: dynamic programming over all 7⁴=2401 possible
 * rows.  For each pair of rows r,s we precompute whether
 * they are compatible.  For a given n the total number of
 * arrays equals the number of length‑(n+1) paths in this
 * directed graph, starting from any state.  The DP
 * transitions are performed iteratively; results are cached
 * so that arbitrary orders of calls are supported.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/abs_int.h"

#define STATE_COUNT 2401   /* 7^4 */

/* Global data structures */
static bool initialized = false;

static short **adj;          /* adjacency lists: adj[r][k] = s */
static unsigned int *adjSize;
static unsigned int *adjCap;
static uint8_t digits[STATE_COUNT][4];   /* base‑7 digits of each row */

static seq_t *dp = NULL;      /* value after last computed step */
static seq_t *newdp = NULL;   /* temporary vector */
static seq_t *sums = NULL;    /* sums[steps] = count with that many steps (rows‑1) */
static idx_t max_cached_n = 0;    /* largest n computed */

/* Convenience: absolute value of int */

/* Build adjacency lists and initialise DP and sums vectors. */
static void init(void)
{
    /* Convert each state id to 4 digits (base 7). */
    for (idx_t r = 0; r < STATE_COUNT; ++r) {
        idx_t v = r;
        for (int col = 3; col >= 0; --col) {
            digits[r][col] = (uint8_t)(v % 7);
            v /= 7;
        }
    }

    /* Allocate adjacency storage. */
    adj      = (short **)malloc(STATE_COUNT * sizeof(short *));
    adjSize  = (unsigned int *)malloc(STATE_COUNT * sizeof(unsigned int));
    adjCap   = (unsigned int *)malloc(STATE_COUNT * sizeof(unsigned int));
    memset(adjSize, 0, STATE_COUNT * sizeof(unsigned int));
    memset(adjCap,  0, STATE_COUNT * sizeof(unsigned int));

    /* Build the graph of compatible row pairs. */
    for (idx_t r = 0; r < STATE_COUNT; ++r) {
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                int val = (int)digits[r][j]       /* a */
                       + (int)digits[s][j + 1]    /* d */
                       - (int)digits[r][j + 1]    /* b */
                       - (int)digits[s][j];       /* c */
                if (abs_int(val) != 1) ok = false;
            }
            if (ok) {
                if (adjCap[r] == adjSize[r]) {
                    unsigned int newcap = adjCap[r] ? adjCap[r] * 2 : 8;
                    adj[r] = (short *)realloc(adj[r], newcap * sizeof(short));
                    adjCap[r] = newcap;
                }
                adj[r][adjSize[r]++] = (short)s;
            }
        }
    }

    /* Allocate DP vectors. */
    dp    = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
    newdp = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));

    /* Initial vector: one way to start from any row. */
    for (idx_t i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)1;

    /* sums[0] holds the count of arrays with 1 row (length‑1). */
    sums = (seq_t *)malloc(sizeof(seq_t));
    sums[0] = (seq_t)STATE_COUNT;
    max_cached_n = 0;
}

/* Extend the cached DP to at least n steps (n ≥ 1). */
static void extend_to(idx_t n)
{
    if (n <= max_cached_n) return;   /* already available */

    sums = (seq_t *)realloc(sums, (n + 1) * sizeof(seq_t));

    for (idx_t step = max_cached_n + 1; step <= n; ++step) {
        /* Zero newdp. */
        memset(newdp, 0, STATE_COUNT * sizeof(seq_t));

        /* Transition from each state. */
        for (idx_t r = 0; r < STATE_COUNT; ++r) {
            seq_t val = dp[r];
            if (!val) continue;
            unsigned int sz = adjSize[r];
            short *neigh = adj[r];
            for (unsigned int k = 0; k < sz; ++k) {
                idx_t tgt = (idx_t)neigh[k];
                newdp[tgt] += val;
            }
        }

        /* Sum the counts for this step. */
        seq_t total = 0;
        for (idx_t i = 0; i < STATE_COUNT; ++i) total += newdp[i];
        sums[step] = total;

        /* Prepare for next step. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }
    max_cached_n = n;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (!initialized) { init(); initialized = true; }
    if (n == 0) return (seq_t)STATE_COUNT;   /* not used for the sequence */
    extend_to(n);
    return sums[n];
}
