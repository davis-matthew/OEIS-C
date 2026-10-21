/*
 * A234186  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..6 such that *
 *            every 2×2 subblock has its diagonal sum differing from its *
 *            antidiagonal sum by exactly 2. The computation is performed by *
 *            dynamic programming over the 343 possible rows of length 3. The
 *            * transition graph between rows is built once...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   abs16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/abs16.h"
 
/* Compile‑time constants for the state space. */
enum { MAX_STATES = 343 };          /* 7³ possible rows */
 
/* Pre‑computed digit decomposition for each state index. */
static uint8_t row0[MAX_STATES];
static uint8_t row1[MAX_STATES];
static uint8_t row2[MAX_STATES];
 
/* Transition adjacency: neigh[p] holds all rows `c` compatible with `p`. */
static uint16_t neigh[MAX_STATES][MAX_STATES];
static uint16_t deg[MAX_STATES];
 
/* Flag indicating whether the transition graph has been initialised. */
static bool initialized = false;
 

 
/* Compatibility test for two consecutive rows. */
static bool compat(uint8_t p0, uint8_t p1, uint8_t p2,
                   uint8_t c0, uint8_t c1, uint8_t c2)
{
    int16_t d1 = (int16_t)(p0 + c1) - (int16_t)(p1 + c0);
    if (abs16(d1) != 2) return false;
    int16_t d2 = (int16_t)(p1 + c2) - (int16_t)(p2 + c1);
    return abs16(d2) == 2;
}
 
/* Build the adjacency graph once. */
static void init_graph(void)
{
    if (initialized) return;
 
    for (int idx = 0; idx < MAX_STATES; ++idx) {
        row0[idx] = (uint8_t)(idx / 49);           /* idx / 7² */
        row1[idx] = (uint8_t)((idx / 7) % 7);      /* (idx / 7) mod 7 */
        row2[idx] = (uint8_t)(idx % 7);
    }
 
    for (int p = 0; p < MAX_STATES; ++p) {
        uint8_t p0 = row0[p];
        uint8_t p1 = row1[p];
        uint8_t p2 = row2[p];
        deg[p] = 0;
        for (int c = 0; c < MAX_STATES; ++c) {
            uint8_t c0 = row0[c];
            uint8_t c1 = row1[c];
            uint8_t c2 = row2[c];
            if (compat(p0, p1, p2, c0, c1, c2)) {
                neigh[p][deg[p]++] = (uint16_t)c;
            }
        }
    }
    initialized = true;
}
 
/* Main function computing a(n) for all n ≥ 1. */
seq_t a(idx_t n)
{
    init_graph();
 
    /* Local DP tables for the current and next row counts. */
    seq_t dp_cur[MAX_STATES];
    seq_t dp_next[MAX_STATES];
 
    /* Initialise: any first row is allowed. */
    for (int i = 0; i < MAX_STATES; ++i) dp_cur[i] = 1;
 
    /* Perform n transition steps (from row 0 to row n). */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < MAX_STATES; ++i) dp_next[i] = 0;
        for (int s = 0; s < MAX_STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val) {
                uint16_t d = deg[s];
                for (uint16_t k = 0; k < d; ++k) {
                    uint16_t t = neigh[s][k];
                    dp_next[t] += val;
                }
            }
        }
        for (int i = 0; i < MAX_STATES; ++i) dp_cur[i] = dp_next[i];
    }
 
    /* Sum over all final states to obtain the total number of arrays. */
    seq_t result = 0;
    for (int i = 0; i < MAX_STATES; ++i) result += dp_cur[i];
    return result;
}
