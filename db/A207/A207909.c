/*
 * A207909  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A207909: number of 4×n binary arrays avoiding the
   horizontal patterns 000 and 010 in any row, and the vertical
   patterns 000 and 011 in any column.  
   The counting is performed by dynamic programming over pairs
   of consecutive columns.  
   Vertical restrictions are encoded in the 9 allowed 4‑bit column
   masks.  Horizontal restrictions depend only on the first
   and last column of a triple, namely that for every row the
   bits of the first and last column cannot both be 0.
   The DP iterates over n−2 column extensions, accumulating
   the number of valid arrays.  The function works for all
   n≥1 in the required order and fits in the 2‑second limit
   for n≈10 000.
*/
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Number of valid columns (vertical constraints) */
    static const uint8_t cols[9] = {
        2,  /* 0010 */
        4,  /* 0100 */
        5,  /* 0101 */
        9,  /* 1001 */
        10, /* 1010 */
        12, /* 1100 */
        13, /* 1101 */
        14, /* 1110 */
        15  /* 1111 */
    };

    static int adj_next[81][9];      /* indices of admissible next states */
    static uint8_t adj_next_count[81];
    static int adj_init = 0;         /* flag whether adjacency precomputation was done */

    /* Pre‑compute transitions between ordered column pairs */
    if (!adj_init) {
        for (int p = 0; p < 9; ++p) {
            for (int c = 0; c < 9; ++c) {
                int state = p * 9 + c;
                adj_next_count[state] = 0;
                for (int n_col = 0; n_col < 9; ++n_col) {
                    /* Horizontal restriction: for every row the bits of
                       the first and last column cannot both be 0. */
                    uint8_t prev_mask = cols[p];
                    uint8_t next_mask = cols[n_col];
                    uint8_t bad = ((~prev_mask) & (~next_mask) & 0xF);
                    if (bad == 0) {
                        int next_state = c * 9 + n_col;
                        adj_next[state][adj_next_count[state]++] = next_state;
                    }
                }
            }
        }
        adj_init = 1;
    }

    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)9;      /* simply the number of allowed columns */
    if (n == 2) return (seq_t)81;     /* any pair of allowed columns */

    /* DP over column triples, implemented with pair states of size 81 */
    seq_t dp[81];
    seq_t nextdp[81];

    /* Initial state (length 2): every pair is admissible */
    for (int i = 0; i < 81; ++i) dp[i] = (seq_t)1;

    /* Extend the array for columns 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 81; ++i) nextdp[i] = (seq_t)0;

        for (int state = 0; state < 81; ++state) {
            seq_t ways = dp[state];
            if (ways == (seq_t)0) continue;
            uint8_t cnt = adj_next_count[state];
            for (int k = 0; k < cnt; ++k) {
                int next_state = adj_next[state][k];
                nextdp[next_state] += ways;
            }
        }
        /* swap dp and nextdp */
        for (int i = 0; i < 81; ++i) dp[i] = nextdp[i];
    }

    /* Sum all ending states to obtain the total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 81; ++i) total += dp[i];
    return total;
}
