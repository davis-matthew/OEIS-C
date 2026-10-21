/*
 * A237490  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 such that for
 *            every 2×2 subblock the value max + upper median – lower median –
 *            min differs by exactly one from that of its horizontal and
 *            vertical neighboring subblocks. Computation is by dynamic
 *            programming over rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   absdiff_int, computeE
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/absdiff_int.h"
#include "../utils/computeE.h"

/* constants */
#define ROWS 64          /* 3-tuples 0..3 */
#define MAXE 7           /* 0..6 */
#define STATE_COUNT (ROWS * MAXE * MAXE)

/* global tables – computed once */
static unsigned char row_digits[ROWS][3];
static unsigned char pairE_left[ROWS][ROWS];
static unsigned char pairE_right[ROWS][ROWS];
static unsigned short transitions[STATE_COUNT][64];
static unsigned char trans_cnt[STATE_COUNT];
static unsigned short init_done = 0;

/* initialise lookup tables */
static void init_tables(void)
{
    if (init_done) return;
    init_done = 1;

    /* enumerate all 3‑tuples (base‑4 numbers) */
    for (int v0 = 0; v0 < 4; ++v0)
        for (int v1 = 0; v1 < 4; ++v1)
            for (int v2 = 0; v2 < 4; ++v2) {
                int idx = (v0 << 4) | (v1 << 2) | v2;
                row_digits[idx][0] = (unsigned char)v0;
                row_digits[idx][1] = (unsigned char)v1;
                row_digits[idx][2] = (unsigned char)v2;
            }

    /* pre‑compute left/right E for every row pair */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j) {
            unsigned char eL = computeE(
                row_digits[i][0], row_digits[i][1],
                row_digits[j][0], row_digits[j][1]);
            unsigned char eR = computeE(
                row_digits[i][1], row_digits[i][2],
                row_digits[j][1], row_digits[j][2]);
            pairE_left[i][j]  = eL;
            pairE_right[i][j] = eR;
        }

    /* pre‑compute transition lists for all states */
    for (unsigned long s = 0; s < STATE_COUNT; ++s) {
        unsigned long p   = s / 49;          /* previous row */
        unsigned long e1  = (s / 7) % 7;     /* E_left  of previous block   */
        unsigned long e2  = s % 7;           /* E_right of previous block   */
        unsigned char cnt = 0;
        for (unsigned long next = 0; next < ROWS; ++next) {
            unsigned char newE1 = pairE_left[p][next];
            unsigned char newE2 = pairE_right[p][next];
            if (absdiff_int((int)newE1, (int)newE2) != 1) continue;
            if (absdiff_int((int)newE1, (int)e1) != 1) continue;
            if (absdiff_int((int)newE2, (int)e2) != 1) continue;
            unsigned long next_state = next * 49 + newE1 * 7 + newE2;
            transitions[s][cnt++] = (unsigned short)next_state;
        }
        trans_cnt[s] = cnt;
    }
}

/* dynamic programming arrays */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* zero DP arrays */
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        dp_cur[i]  = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }

    /* initial two rows */
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1) {
            unsigned char eL = pairE_left[r0][r1];
            unsigned char eR = pairE_right[r0][r1];
            if (absdiff_int((int)eL, (int)eR) != 1) continue;
            unsigned long idx = r1 * 49 + eL * 7 + eR;
            dp_cur[idx] += (seq_t)1;
        }

    if (n == 1) {
        /* sum all valid two‑row arrays */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp_cur[i];
        return total;
    }

    /* add further rows (n‑1 more rows) */
    for (idx_t step = 1; step < n; ++step) {
        /* clear dp_next */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        /* transitions */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == (seq_t)0) continue;
            unsigned char tc = trans_cnt[s];
            for (unsigned char t = 0; t < tc; ++t) {
                unsigned short ns = transitions[s][t];
                dp_next[ns] += ways;
            }
        }

        /* copy dp_next to dp_cur for next iteration */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* final sum */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < STATE_COUNT; ++i) total += dp_cur[i];
    return total;
}
