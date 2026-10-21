/*
 * A206937  Number of 4Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary matrices avoiding horizontal 0 0 0
 *            and vertical 0 1 1. Computation via dynamic programming over
 *            columns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Allowed column masks (4-bit) that contain no vertical 0 1 1 */
static const uint8_t allowed[12] = {
    0x0, /* 0000 */
    0x1, /* 0001 */
    0x2, /* 0010 */
    0x4, /* 0100 */
    0x5, /* 0101 */
    0x8, /* 1000 */
    0x9, /* 1001 */
    0xA, /* 1010 */
    0xC, /* 1100 */
    0xD, /* 1101 */
    0xE, /* 1110 */
    0xF  /* 1111 */
};

/* DP tables: dp[i*12 + j] holds number of ways to build a 4×len
   array whose last two columns are masks i and j (indices 0..11). */
static seq_t dp_curr[12 * 12];
static seq_t dp_next[12 * 12];

seq_t a(idx_t n)
{
    /* Handle small offsets directly */
    if (n == 0) {
        return (seq_t)0;          /* undefined, but safe */
    }
    if (n == 1) {
        return (seq_t)12;
    }
    if (n == 2) {
        return (seq_t)144;
    }

    /* Initialise DP for length 2 (all 12×12 pairs are allowed) */
    for (size_t i = 0; i < 12; ++i) {
        for (size_t j = 0; j < 12; ++j) {
            dp_curr[i * 12 + j] = (seq_t)1;
        }
    }

    seq_t total = 0;

    /* Extend DP up to length n */
    for (idx_t len = 3; len <= n; ++len) {
        /* Clear next table */
        for (size_t idx = 0; idx < 144; ++idx) {
            dp_next[idx] = (seq_t)0;
        }

        /* Transition: add new column k behind existing ending (i,j) */
        for (size_t i = 0; i < 12; ++i) {
            for (size_t j = 0; j < 12; ++j) {
                seq_t val = dp_curr[i * 12 + j];
                if (val == (seq_t)0) {
                    continue;
                }
                for (size_t k = 0; k < 12; ++k) {
                    /* Horizontal 0 0 0 avoidance: every row must have a 1
                       in at least one of the three consecutive columns. */
                    if ((allowed[i] | allowed[j] | allowed[k]) == 0xF) {
                        dp_next[j * 12 + k] += val;
                    }
                }
            }
        }

        /* Sum all sequences of current length */
        seq_t sum = (seq_t)0;
        for (size_t idx = 0; idx < 144; ++idx) {
            sum += dp_next[idx];
        }

        /* Prepare for next iteration */
        for (size_t idx = 0; idx < 144; ++idx) {
            dp_curr[idx] = dp_next[idx];
        }
        total = sum; /* cur len is now 'len' */
    }

    return total;
}
