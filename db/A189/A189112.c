/*
 * A189112  Number of 4Xn binary arrays without the pattern 1 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A189112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS sequence A189112: Number of 4×n binary arrays without the pattern
 *  1 1 0 diagonally or anti‑diagonally.
 *
 *  The implementation uses dynamic programming over column pairs.
 *  States: a pair of successive 4‑bit columns (a,b).  There are 256 states.
 *  Transition: for every third column c we check the forbidden
 *  patterns between (a,b,c).  The number of arrays of width n is the
 *  sum of all state counts after processing n columns.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Check whether a triple of 4‑bit columns (a,b,c) contains a forbidden
 * 1 1 0 pattern either on the main diagonal or the anti‑diagonal. */
static bool allowed_transition(unsigned char a, unsigned char b,
                               unsigned char c)
{
    /* Scan the two possible 3‑row blocks: r = 0 and r = 1. */
    for (int r = 0; r <= 1; ++r) {
        /* Main diagonal 1 1 0 */
        int bit_a = (a >> r)       & 1;
        int bit_b = (b >> (r+1))   & 1;
        int bit_c = (c >> (r+2))   & 1;
        if (bit_a == 1 && bit_b == 1 && bit_c == 0) return false;

        /* Anti‑diagonal 1 1 0 */
        int bit_c2 = (c >> r)       & 1;
        int bit_b2 = (b >> (r+1))   & 1;
        int bit_a2 = (a >> (r+2))   & 1;
        if (bit_c2 == 1 && bit_b2 == 1 && bit_a2 == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used in OEIS, but harmless */
    if (n == 1) return (seq_t)16;         /* width 1: 2^4 arrays */
    if (n == 2) return (seq_t)256;        /* width 2: 2^(4×2) arrays */

    const int STATES = 256;               /* 16 × 16 states */
    seq_t dp_curr[STATES];
    seq_t dp_next[STATES];

    /* Initialise – every pair of first two columns is allowed. */
    for (int i = 0; i < STATES; ++i) {
        dp_curr[i] = (seq_t)1;
        dp_next[i] = (seq_t)0;
    }

    /* Process columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* Reset next state counts */
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        /* Enumerate all current states (b,a) and move to (c,b). */
        for (int b = 0; b < 16; ++b) {
            for (int a = 0; a < 16; ++a) {
                seq_t cur = dp_curr[b * 16 + a];
                if (cur == (seq_t)0) continue;

                for (int c = 0; c < 16; ++c) {
                    if (!allowed_transition(a, b, c)) continue;
                    int idx = c * 16 + b;          /* new state (c,b) */
                    dp_next[idx] = dp_next[idx] + cur;
                }
            }
        }

        /* Copy next counts into current for the next iteration. */
        for (int i = 0; i < STATES; ++i) dp_curr[i] = dp_next[i];
    }

    /* Sum all states to get the total for width n. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i) total = total + dp_curr[i];
    return total;
}
