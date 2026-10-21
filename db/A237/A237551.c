/*
 * A237551  Number of (n+1)X(2+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined implicitly by dynamic programming over pairs of
 *            consecutive rows of a 3‑column array of height n+1. For every
 *            pair of rows s,t let d0[s][t] = max−upper median of the 2×2
 *            subblock in columns 0–1, d1[s][t] = max−upper median of the 2×2
 *            subblock in columns 1–2. An array is valid iff...
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   diff_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/diff_four.h"

#define NUM_ROWS 27

static uint8_t digits[NUM_ROWS][3];
static uint8_t diff0[NUM_ROWS][NUM_ROWS];
static uint8_t diff1[NUM_ROWS][NUM_ROWS];
static bool initialized = false;

static inline int diff_abs(uint8_t a, uint8_t b)
{
    return a > b ? (int)(a - b) : (int)(b - a);
}

static void compute_tables(void)
{
    if (initialized) return;
    for (int s = 0; s < NUM_ROWS; ++s) {
        int x = s;
        digits[s][0] = x / 9;
        digits[s][1] = (x / 3) % 3;
        digits[s][2] = x % 3;
    }
    for (int s = 0; s < NUM_ROWS; ++s) {
        for (int t = 0; t < NUM_ROWS; ++t) {
            diff0[s][t] = diff_four(digits[s][0], digits[s][1],
                                    digits[t][0], digits[t][1]);
            diff1[s][t] = diff_four(digits[s][1], digits[s][2],
                                    digits[t][1], digits[t][2]);
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_tables();

    static seq_t dp_cur[NUM_ROWS][NUM_ROWS];
    static seq_t dp_next[NUM_ROWS][NUM_ROWS];

    /* initialise dp_cur for height = 2 (first pair of rows) */
    for (int s = 0; s < NUM_ROWS; ++s) {
        for (int t = 0; t < NUM_ROWS; ++t) {
            dp_cur[s][t] = (diff_abs(diff0[s][t], diff1[s][t]) == 1) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* perform n-1 further transitions to reach height n+1 */
    for (idx_t r = 2; r <= n; ++r) {
        /* clear dp_next */
        for (int i = 0; i < NUM_ROWS; ++i)
            for (int j = 0; j < NUM_ROWS; ++j)
                dp_next[i][j] = (seq_t)0;

        for (int s = 0; s < NUM_ROWS; ++s) {
            for (int t = 0; t < NUM_ROWS; ++t) {
                seq_t val = dp_cur[s][t];
                if (!val) continue;
                for (int u = 0; u < NUM_ROWS; ++u) {
                    if (diff_abs(diff0[s][t], diff0[t][u]) == 1 &&
                        diff_abs(diff1[s][t], diff1[t][u]) == 1 &&
                        diff_abs(diff0[t][u], diff1[t][u]) == 1) {
                        dp_next[t][u] += val;
                    }
                }
            }
        }

        /* copy dp_next to dp_cur for next iteration */
        for (int i = 0; i < NUM_ROWS; ++i)
            for (int j = 0; j < NUM_ROWS; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* sum all possible ending states */
    seq_t result = (seq_t)0;
    for (int s = 0; s < NUM_ROWS; ++s)
        for (int t = 0; t < NUM_ROWS; ++t)
            result += dp_cur[s][t];

    return result;
}
