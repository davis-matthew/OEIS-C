/*
 * A180754  Half the number of nX5 binary arrays with each element equal to at
 *          least two neighbors.
 * https://oeis.org/A180754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A180754
 *   Half the number of n×5 binary arrays with each element equal to 
 *   at least two neighbors.  DP with row‑by‑row transition.
 *
 *   Algorithm:  for each possible current and previous row we keep a
 *   count of arrays ending there.  There are 32 possible rows, plus a
 *   sentinel “none” for boundaries.  The transition table is pre‑
 *   computed once.  The total number of valid arrays of height n is
 *   summed over all valid terminal states and divided by two.
 *
 *   The implementation works for any seq_t that supports +,-,*,/ and
 *   a ctor from an integral constant.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef NONE
#define NONE 32          /* sentinel for a missing row (top/bottom) */
#endif

/* Pre‑computation tables */
static uint8_t      valid_next[33][32][32];  /* lists of next rows for each (up,mid) */
static uint8_t      next_count[33][32];
static bool         valid_mid[33][32][33];
static bool         init_done = false;

/* Helper to check if a middle row `mid` is valid given its up and down rows */
static bool check_row(int up, int mid, int down)
{
    int bits[5];
    for (int c = 0; c < 5; ++c)
        bits[c] = (mid >> c) & 1;
    for (int c = 0; c < 5; ++c) {
        int v = bits[c];
        int cnt = 0;
        if (up != NONE && ((up >> c) & 1) == v) ++cnt;
        if (down != NONE && ((down >> c) & 1) == v) ++cnt;
        if (c > 0   && bits[c-1] == v) ++cnt;
        if (c < 4   && bits[c+1] == v) ++cnt;
        if (cnt < 2) return false; /* fails */
    }
    return true;
}

/* Initialise tables (called once) */
static void init_tables(void)
{
    if (init_done) return;
    for (int up = 0; up <= NONE; ++up) {
        for (int mid = 0; mid < 32; ++mid) {
            int ci = 0;
            for (int down = 0; down <= NONE; ++down) {
                bool ok = check_row(up, mid, down);
                valid_mid[up][mid][down] = ok;
                if (ok && ci < 32) {
                    valid_next[up][mid][ci++] = (uint8_t)down;
                }
            }
            next_count[up][mid] = ci;
        }
    }
    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return (seq_t)0;          /* below offset, arbitrary */

    /* DP arrays: index = up*32 + mid, where up∈[0..32], mid∈[0..31] */
    static seq_t cur[33 * 32], nxt[33 * 32];
    size_t total_states = 33 * 32;

    /* initialise with first row only: (NONE, row) = 1 for all rows */
    for (size_t i = 0; i < total_states; ++i) cur[i] = (seq_t)0;
    for (int r = 0; r < 32; ++r) {
        cur[NONE * 32 + r] = (seq_t)1;
    }

    /* Process rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        for (size_t i = 0; i < total_states; ++i) nxt[i] = (seq_t)0;
        for (int up = 0; up <= NONE; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                size_t id = up * 32 + mid;
                seq_t cur_val = cur[id];
                if (cur_val == (seq_t)0) continue;
                uint8_t cnt = next_count[up][mid];
                const uint8_t *list = valid_next[up][mid];
                for (int k = 0; k < cnt; ++k) {
                    int nxtrow = list[k];
                    size_t nid = mid * 32 + nxtrow;
                    nxt[nid] += cur_val;
                }
            }
        }
        /* swap cur and nxt */
        for (size_t i = 0; i < total_states; ++i) cur[i] = nxt[i];
    }

    /* Sum valid final states (last row must be valid with down = NONE) */
    seq_t total = (seq_t)0;
    for (int up = 0; up <= NONE; ++up) {
        for (int mid = 0; mid < 32; ++mid) {
            size_t id = up * 32 + mid;
            seq_t val = cur[id];
            if (val == (seq_t)0) continue;
            if (valid_mid[up][mid][NONE]) {
                total += val;
            }
        }
    }

    /* sequence is half the count */
    return total / (seq_t)2;
}
