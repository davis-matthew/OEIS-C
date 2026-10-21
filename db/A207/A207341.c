/*
 * A207341  Number of nX3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes sequence A207341:
   a(n) = number of n×3 binary arrays that avoid the patterns
          horizontally: 000 and 011
          vertically: 000 and 111
   The algorithm uses a dynamic programming approach over the
   ordered pair of the last two rows.  Each row is a 3‑bit integer
   (0..7).  Allowed rows are all except 0 (000) and 3 (011).
   There are 64 possible ordered pairs of rows.  For each step
   the DP updates counts of valid sequences of that length.
   This method works in O(n) time with a constant factor of ~384
   operations per row, which easily satisfies the 2‑second limit
   for n up to about 10 000. */

#include <stdint.h>

static const unsigned char allowed_rows[6] = {1,2,4,5,6,7};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset, return 0 */
    if (n == 1) return (seq_t)6;          /* 6 allowed rows */
    if (n == 2) return (seq_t)36;         /* 6×6 pairings */

    /* DP arrays for counts of sequences ending with a given pair (r_{i-1}, r_i) */
    seq_t prev[64];
    seq_t curr[64];

    /* initialise prev for n = 2: all ordered pairs of allowed rows */
    for (int i = 0; i < 64; ++i) prev[i] = 0;
    for (int r1i = 0; r1i < 6; ++r1i) {
        unsigned char r1 = allowed_rows[r1i];
        for (int r2i = 0; r2i < 6; ++r2i) {
            unsigned char r2 = allowed_rows[r2i];
            prev[(r1 << 3) | r2] = (seq_t)1;
        }
    }

    /* iterate rows 3 to n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 64; ++i) curr[i] = 0;
        for (int state = 0; state < 64; ++state) {
            seq_t cnt = prev[state];
            if (cnt == 0) continue;
            unsigned char p2 = state >> 3;
            unsigned char p1 = state & 7;
            for (int ri = 0; ri < 6; ++ri) {
                unsigned char r = allowed_rows[ri];
                int ok = 1;
                for (int col = 0; col < 3; ++col) {
                    int b2 = (p2 >> col) & 1;
                    int b1 = (p1 >> col) & 1;
                    int bc = (r   >> col) & 1;
                    int t = (b2 << 2) | (b1 << 1) | bc;   /* triple bits */
                    if (t == 0 || t == 7) { ok = 0; break; } /* 000 or 111 */
                }
                if (ok) {
                    int newstate = (p1 << 3) | r;
                    curr[newstate] += cnt;
                }
            }
        }
        /* copy curr into prev for next iteration */
        for (int i = 0; i < 64; ++i) prev[i] = curr[i];
    }

    /* sum all states to get total sequences of length n */
    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += prev[i];
    return result;
}
