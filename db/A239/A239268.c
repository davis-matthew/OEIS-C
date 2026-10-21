/*
 * A239268  Number of domicule tilings of a 6 X n grid.
 * https://oeis.org/A239268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A239268: Number of domicule tilings of a 6×n grid.
   Recurrence:  a(0)=1.
   For n>0, a(n) is obtained by dynamic programming over column states.
   A 64×64 transition matrix T is precomputed once:
   T[prev][next] = number of ways to fill a column when the cells
   already occupied by tiles coming from the previous column are
   described by the bitmask 'prev', and the cells that will be occupied
   in the next column are described by 'next'.
   The DP iterates n columns and keeps a vector of size 64 over state
   masks.  The result is the entry for mask 0. */

#include <stdint.h>

static void dfs(int prev, int currMask, int nextMask, seq_t T[64][64]) {
    int r;
    for (r = 0; r < 6; ++r) {
        if (!(currMask & (1 << r))) break;
    }
    if (r == 6) {
        T[prev][nextMask] = T[prev][nextMask] + (seq_t)1;
        return;
    }
    /* vertical domino */
    if (r + 1 < 6 && !(currMask & (1 << (r + 1)))) {
        dfs(prev, currMask | (1 << r) | (1 << (r + 1)), nextMask, T);
    }
    /* horizontal domino to same row in next column */
    if (!(nextMask & (1 << r))) {
        dfs(prev, currMask | (1 << r), nextMask | (1 << r), T);
    }
    /* diagonal pair to row r+1 in next column */
    if (r + 1 < 6 && !(nextMask & (1 << (r + 1)))) {
        dfs(prev, currMask | (1 << r), nextMask | (1 << (r + 1)), T);
    }
    /* diagonal pair to row r-1 in next column */
    if (r > 0 && !(nextMask & (1 << (r - 1)))) {
        dfs(prev, currMask | (1 << r), nextMask | (1 << (r - 1)), T);
    }
}

static void build_transitions(seq_t T[64][64]) {
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            T[i][j] = (seq_t)0;
    for (int prev = 0; prev < 64; ++prev) {
        dfs(prev, prev, 0, T);
    }
}

seq_t a(idx_t n) {
    static seq_t T[64][64];
    static int initialized = 0;
    if (!initialized) {
        build_transitions(T);
        initialized = 1;
    }

    seq_t dp[64];
    seq_t ndp[64];
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) ndp[i] = (seq_t)0;
        for (int prev = 0; prev < 64; ++prev) {
            if (dp[prev] == (seq_t)0) continue;
            for (int next = 0; next < 64; ++next) {
                if (T[prev][next] == (seq_t)0) continue;
                seq_t prod = dp[prev] * T[prev][next];
                ndp[next] = ndp[next] + prod;
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    return dp[0];
}
