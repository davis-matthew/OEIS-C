/*
 * A234440  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+1)×6 arrays with entries 0..2 in which every 2×2
 *            subblock has its diagonal sum differing from its antidiagonal
 *            sum by 1. The implementation builds the transition graph on 3^6
 *            states and runs a standard transfer‑matrix DP
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* 3^6 = 729 possible rows of length 6 over {0,1,2}.  */
#define STATES 729

/* Transition data: adj_start[p] is the first index in adj_edges[],
   adj_count[p] is how many neighbors state p has.  */
static int *adj_start   = NULL;   /* size STATES  */
static int *adj_count   = NULL;   /* size STATES  */
static int *adj_edges   = NULL;   /* size total_edges */

/* DP buffers: cur holds counts for the current height, next for the next. */
static seq_t *cur  = NULL;
static seq_t *next = NULL;

/* Cache of computed a(n) values.  */
static seq_t *cache       = NULL;
static idx_t  cache_size  = 0;     /* number of slots currently allocated */
static idx_t  max_cached  = 0;     /* highest n whose a(n) is cached  */

/* Number of rows for which cur holds a valid DP vector. */
static idx_t current_rows = 0;

/* --------------------------------------------------------------------- */
static void init(void)
{
    /* Build the transition graph only once.  */
    if (adj_start != NULL)
        return;   /* already initialised */

    /* Unpack all 729 states as 6‑digit base‑3 numbers. */
    static unsigned char digits[STATES][6];
    for (int p = 0; p < STATES; ++p) {
        int tmp = p;
        for (int k = 5; k >= 0; --k) {
            digits[p][k] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* --- Count edges per state --- */
    adj_count = (int *)calloc(STATES, sizeof(int));
    if (!adj_count) exit(EXIT_FAILURE);

    for (int p = 0; p < STATES; ++p) {
        unsigned char *dp = digits[p];
        for (int r = 0; r < STATES; ++r) {
            unsigned char *dr = digits[r];
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                int sum1 = dp[j]     + dr[j + 1];
                int sum2 = dp[j + 1] + dr[j];
                if (abs(sum1 - sum2) != 1)
                    ok = false;
            }
            if (ok)
                ++adj_count[p];
        }
    }

    /* --- Compute start indices and total number of edges --- */
    adj_start = (int *)malloc(STATES * sizeof(int));
    if (!adj_start) exit(EXIT_FAILURE);
    int total_edges = 0;
    for (int i = 0; i < STATES; ++i) {
        adj_start[i] = total_edges;
        total_edges += adj_count[i];
    }

    adj_edges = (int *)malloc(total_edges * sizeof(int));
    if (!adj_edges) exit(EXIT_FAILURE);

    /* --- Fill adjacency lists --- */
    int *pos = (int *)malloc(STATES * sizeof(int));
    if (!pos) exit(EXIT_FAILURE);
    memcpy(pos, adj_start, STATES * sizeof(int));

    for (int p = 0; p < STATES; ++p) {
        unsigned char *dp = digits[p];
        for (int r = 0; r < STATES; ++r) {
            unsigned char *dr = digits[r];
            bool ok = true;
            for (int j = 0; j < 5 && ok; ++j) {
                int sum1 = dp[j]     + dr[j + 1];
                int sum2 = dp[j + 1] + dr[j];
                if (abs(sum1 - sum2) != 1)
                    ok = false;
            }
            if (ok)
                adj_edges[pos[p]++] = r;
        }
    }
    free(pos);

    /* --- Allocate DP buffers --- */
    cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
    next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!cur || !next) exit(EXIT_FAILURE);

    /* Initial DP vector (height = 1): one way per row. */
    for (int i = 0; i < STATES; ++i)
        cur[i] = (seq_t)1;

    /* Initialise cache. */
    cache_size = 2;                    /* indices 0 .. 1  */
    cache      = (seq_t *)calloc(cache_size, sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    max_cached = 0;
    current_rows = 1;   /* cur holds arrays of height 1 initially */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)               /* outside offset */
        return (seq_t)0;

    init();                   /* ensure data structures exist */

    /* Make cache large enough for index n. */
    size_t needed = (size_t)n + 1;   /* need indices 0..n */
    if (needed > cache_size) {
        cache_size = needed;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Return cached value if already computed. */
    if (n <= max_cached)
        return cache[n];

    /* Extend DP from current_rows up to n+1. */
    while (current_rows < n + 1) {
        /* Clear next vector. */
        memset(next, 0, STATES * sizeof(seq_t));

        /* Transition from cur to next. */
        for (int p = 0; p < STATES; ++p) {
            seq_t val = cur[p];
            if (val == (seq_t)0)
                continue;
            int s = adj_start[p];
            int e = s + adj_count[p];
            for (int idx = s; idx < e; ++idx) {
                int r = adj_edges[idx];
                next[r] += val;
            }
        }

        /* Increment height counter. */
        ++current_rows;

        /* Sum all ways to reach height = current_rows. */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < STATES; ++i)
            sum += next[i];
        cache[(size_t)(current_rows - 1)] = sum;   /* a(current_rows-1) */

        /* Swap buffers for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    max_cached = n;
    return cache[n];
}
