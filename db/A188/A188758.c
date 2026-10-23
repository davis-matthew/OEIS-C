/*
 * A188758  Number of nX4 binary arrays without the pattern 0 0 1 vertically
 *          or horizontally.
 * https://oeis.org/A188758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays avoiding the pattern 0 0 1
 *            vertically or horizontally
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 … 2 the values are 12 and 144; for n ≥ 3
   the value is computed by a transfer‑matrix DP over the 12
   admissible 4‑bit rows.  */

#include <stdbool.h>
#include <stdint.h>

enum { NUM_ROWS = 12 };

/* 4‑bit rows that contain no horizontal 0 0 1. */
static const uint8_t row_bits[NUM_ROWS] =
{ 0b0000, 0b0100, 0b0101, 0b0110, 0b0111,
  0b1000, 0b1010, 0b1011, 0b1100, 0b1101,
  0b1110, 0b1111 };

/* allowed[a][b][c] == 1 if adding row c after rows a,b does
   not create a vertical 0 0 1.  */
static uint8_t allowed[NUM_ROWS][NUM_ROWS][NUM_ROWS];

/* Initialise the allowed table on first use.  */
static bool init_allowed(void)
{
    for (int a = 0; a < NUM_ROWS; ++a)
        for (int b = 0; b < NUM_ROWS; ++b)
            for (int c = 0; c < NUM_ROWS; ++c) {
                uint8_t ok = 1;
                for (int col = 0; col < 4; ++col) {
                    uint8_t bit_a = (row_bits[a] >> col) & 1;
                    uint8_t bit_b = (row_bits[b] >> col) & 1;
                    uint8_t bit_c = (row_bits[c] >> col) & 1;
                    if (!bit_a && !bit_b && bit_c) { ok = 0; break; }
                }
                allowed[a][b][c] = ok;
            }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)(12ULL * 12ULL);   /* 144 */

    /* Initialise the transition table once. */
    static bool initialized = false;
    if (!initialized) { init_allowed(); initialized = true; }

    /* DP arrays: dp[a][b] is the number of arrays of current length
       ending with rows a (second‑to‑last) and b (last).   */
    seq_t dp[NUM_ROWS][NUM_ROWS];
    seq_t ndp[NUM_ROWS][NUM_ROWS];

    /* Base for length 2: every pair of admissible rows is allowed. */
    for (int a = 0; a < NUM_ROWS; ++a)
        for (int b = 0; b < NUM_ROWS; ++b)
            dp[a][b] = (seq_t)1;

    /* Extend the arrays to length n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero ndp. */
        for (int a = 0; a < NUM_ROWS; ++a)
            for (int b = 0; b < NUM_ROWS; ++b)
                ndp[a][b] = (seq_t)0;

        /* Transition from (a,b) to (b,c). */
        for (int a = 0; a < NUM_ROWS; ++a) {
            for (int b = 0; b < NUM_ROWS; ++b) {
                seq_t ways = dp[a][b];
                if (!ways) continue;
                for (int c = 0; c < NUM_ROWS; ++c)
                    if (allowed[a][b][c])
                        ndp[b][c] += ways;
            }
        }

        /* Copy ndp back to dp for the next iteration. */
        for (int a = 0; a < NUM_ROWS; ++a)
            for (int b = 0; b < NUM_ROWS; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum over all ending pairs. */
    seq_t total = (seq_t)0;
    for (int a = 0; a < NUM_ROWS; ++a)
        for (int b = 0; b < NUM_ROWS; ++b)
            total += dp[a][b];

    return total;
}
