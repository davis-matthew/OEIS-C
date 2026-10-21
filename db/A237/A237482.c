/*
 * A237482  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A237482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237482: number of (n+1)×2 arrays with entries 0..3 in which
 * for every 2×2 subblock the value max+min−lower_median is the same
 * for all subblocks.  This implementation uses dynamic programming
 * on a graph of 16 row states and the four possible constant values
 * K = 0,1,2,3.  The number of arrays of length L = n+1 rows is
 * obtained by summing all walks of length L−1 that keep the same K
 * across all transitions.  The routine stores results in a static
 * cache to answer repeated queries efficiently.  Complexity is
 * O(16²·4·max_n)≈10⁷ elementary operations for max_n≈10⁴.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Adjacency matrices for each possible K value (0..3).  */
static unsigned char adj[4][16][16] = {0};

/* Compute K for a pair of consecutive rows encoded as states s1, s2.  */
static int compute_k(int a0, int b0, int a1, int b1) {
    int v[4] = {a0, b0, a1, b1};
    /* insertion sort of 4 elements */
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (v[i] > v[j]) {
                int t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
        }
    }
    return v[3] + v[0] - v[1]; /* max + min - lower median */
}

/* Initialise the adjacency matrices once.  */
static void init_adj(void) {
    static int done = 0;
    if (done) return;
    done = 1;
    for (int s1 = 0; s1 < 16; ++s1) {
        int a0 = (s1 >> 2) & 3;
        int b0 = s1 & 3;
        for (int s2 = 0; s2 < 16; ++s2) {
            int a1 = (s2 >> 2) & 3;
            int b1 = s2 & 3;
            int k = compute_k(a0, b0, a1, b1);
            if (k >= 0 && k <= 3) {
                adj[k][s1][s2] = 1;
            }
        }
    }
}

/* Main function requested by the exercise.  */
seq_t a(idx_t n)
{
    /* Cache for computed a(n) values.  index 0 is unused.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;     /* allocated size */
    static size_t cache_len = 0;      /* highest n for which a(n) is stored */

    /* Dynamic programming state for lengths from 1 to current.  */
    static seq_t dp_cur[4][16] = {{0}};
    static seq_t dp_next[4][16] = {{0}};
    static size_t cur_len = 1;        /* current number of rows stored in dp_cur */

    /* Initialise structures on the first call.  */
    if (!cache) {
        /* allocate space for first few values; will grow as needed */
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;        /* a(0) unused */
        /* Initialise dp_cur: all one‑row states are possible for every K */
        for (int k = 0; k < 4; ++k)
            for (int s = 0; s < 16; ++s)
                dp_cur[k][s] = 1;
        init_adj();
    }

    /* Ensure cache can hold a(n).  */
    if ((idx_t)cache_len < n) {
        size_t needed = (size_t)n + 2;   /* +1 for length, +1 for a(0) */
        if (cache_size < needed) {
            size_t new_size = cache_size * 2;
            if (new_size < needed) new_size = needed;
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            cache_size = new_size;
        }
        /* Extend DP to required length (rows = n+1).  */
        size_t target_len = (size_t)n + 1;
        for (size_t step = cur_len; step < target_len; ++step) {
            /* compute dp_next = dp_cur * adj */
            for (int k = 0; k < 4; ++k) {
                for (int s2 = 0; s2 < 16; ++s2) {
                    seq_t sum = 0;
                    for (int s1 = 0; s1 < 16; ++s1) {
                        if (adj[k][s1][s2]) {
                            sum += dp_cur[k][s1];
                        }
                    }
                    dp_next[k][s2] = sum;
                }
            }
            /* copy dp_next to dp_cur */
            for (int k = 0; k < 4; ++k)
                for (int s = 0; s < 16; ++s)
                    dp_cur[k][s] = dp_next[k][s];

            /* total number of arrays for this length (n = step) */
            seq_t total = 0;
            for (int k = 0; k < 4; ++k)
                for (int s = 0; s < 16; ++s)
                    total += dp_cur[k][s];

            cache[step] = total;      /* step = n + 1, so store at index n+1 */
            cache_len = step;
        }
        cur_len = target_len;
    }

    return cache[n];
}
