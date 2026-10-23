/*
 * A028482  Number of perfect matchings in graph C_{11} X P_{2n}.
 * https://oeis.org/A028482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect matchings in C11 × P2n. Computed by
 *            transfer‑matrix DP over the 11‑vertex cycle
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   least_bit_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/least_bit_index.h"

#define H 11                     /* height of the cycle */
#define STATES (1 << H)          /* 2048 possible masks on a column */
#define MAX_TRANS 700            /* safe upper bound on transitions per state */

/* Transition tables: for each previous mask we store all possible next masks. */
static uint16_t trans_next[STATES][MAX_TRANS];
static unsigned trans_count[STATES];
static int trans_inited = 0;

/* Recursive enumeration of matchings on one column.
   prevMask   – mask of vertices already matched to the left.
   freeMask   – vertices free in the current column (not matched left).
   nextMask   – vertices that will be matched to the right.
   The function records every possible nextMask into trans_next[prevMask]. */
static void dfs_enum(int prevMask, unsigned freeMask, unsigned nextMask) {
    if (freeMask == 0u) {                     /* no free vertices left */
        trans_next[prevMask][ trans_count[prevMask]++ ] = (uint16_t)nextMask;
        return;
    }
    int i = least_bit_index(freeMask);        /* first free vertex */
    unsigned rem = freeMask & ~(1u << i);

    /* Option 1: match i vertically (to the right). */
    dfs_enum(prevMask, rem, nextMask | (1u << i));

    /* Option 2: match i with its clockwise neighbour. */
    int j1 = (i + 1) % H;
    if (rem & (1u << j1))
        dfs_enum(prevMask, rem & ~(1u << j1), nextMask);

    /* Option 3: match i with its counter‑clockwise neighbour. */
    int j2 = (i + H - 1) % H;
    if (rem & (1u << j2))
        dfs_enum(prevMask, rem & ~(1u << j2), nextMask);
}

/* Build the transition tables once. */
static void init_transitions(void) {
    if (trans_inited) return;
    trans_inited = 1;
    for (int prev = 0; prev < STATES; ++prev) {
        trans_count[prev] = 0;
        unsigned freeMask = (~(unsigned)prev) & (STATES - 1);
        dfs_enum(prev, freeMask, 0u);
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_transitions();

    static seq_t dp[2][STATES];
    int cur = 0, nxt = 1;

    /* initial state: nothing matched to the left */
    memset(dp[cur], 0, sizeof(dp[cur]));
    dp[cur][0] = 1;

    idx_t steps = 2 * n;                 /* number of columns */
    for (idx_t step = 0; step < steps; ++step) {
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (int prev = 0; prev < STATES; ++prev) {
            seq_t val = dp[cur][prev];
            if (!val) continue;
            unsigned cnt = trans_count[prev];
            for (unsigned k = 0; k < cnt; ++k) {
                uint16_t nextMask = trans_next[prev][k];
                dp[nxt][nextMask] += val;
            }
        }
        /* swap roles of the two DP layers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }
    return dp[cur][0];   /* final state must have no unmatched vertices */
}
