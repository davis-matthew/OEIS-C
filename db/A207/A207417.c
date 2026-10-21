/*
 * A207417  Number of nX6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207417: Number of n×6 binary arrays avoiding horizontal patterns
    001 and 010, and vertical patterns 011 and 101.
    Computation is by dynamic programming on pairs of consecutive rows.
    Transition matrix size is 21×21 (rows = 21 allowed 6‑bit patterns).
    Each state is a pair (prev2, prev1); transitions add a new row
    compatible with both horizontal rules and the vertical 3‑row rule.
    For n=1 answer = 21; for n=2 answer = 21²; for n≥3 DP as described.
*/

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Number of allowed 6‑bit rows */
    static uint8_t row_bits[64];
    static uint8_t row_id[64];
    static size_t row_cnt = 0;
    /* State arrays */
    static seq_t dp_prev[64*64];
    static seq_t dp_next[64*64];
    static unsigned short adj_dst[64*64][21];
    static unsigned char  adj_len[64*64];
    static bool initialized = false;

    if (!initialized) {
        /* Build list of allowed 6‑bit rows (no 001 or 010 horizontally) */
        for (uint8_t p = 0; p < 64; ++p) {
            bool ok = true;
            /* check all 3‑bit windows in the 6 bits (positions 0‑3) */
            for (int k = 0; k <= 3 && ok; ++k) {
                uint8_t w = (p >> k) & 7;        /* 3 bits */
                if (w == 0x1 || w == 0x2)        /* 001 or 010 */
                    ok = false;
            }
            if (ok) {
                row_id[p] = (uint8_t)row_cnt;
                row_bits[row_cnt] = p;
                ++row_cnt;
            } else {
                row_id[p] = 255;
            }
        }
        /* Pre‑compute transitions between state (a,b) -> (b,c) */
        size_t M = row_cnt;               /* 21 */
        size_t states = M * M;            /* 441 */
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < M; ++j) {
                size_t s = i * M + j;
                unsigned char c = 0;
                uint8_t a = row_bits[i];
                uint8_t b = row_bits[j];
                for (size_t k = 0; k < M; ++k) {
                    uint8_t cbit = row_bits[k];
                    /* check vertical rule for all 6 columns */
                    bool ok = true;
                    for (int col = 0; col < 6 && ok; ++col) {
                        uint8_t A = (a >> col) & 1;
                        uint8_t B = (b >> col) & 1;
                        uint8_t C = (cbit >> col) & 1;
                        if (C && ((A == 0 && B == 1) || (A == 1 && B == 0)))
                            ok = false;
                    }
                    if (ok) {
                        adj_dst[s][c] = (unsigned short)(j * M + k);
                        ++c;
                    }
                }
                adj_len[s] = c;
            }
        }
        initialized = true;
    }

    size_t M = row_cnt;               /* 21 */
    size_t states = M * M;            /* 441 */

    if (n == 0) return 0;             /* beyond offset – undefined, safe */
    if (n == 1) return (seq_t)M;      /* 21 */
    if (n == 2) return (seq_t)M * M;  /* 441 */

    /* Initialize dp for two rows: each pair has exactly one way */
    for (size_t s = 0; s < states; ++s) dp_prev[s] = 1;

    for (idx_t row = 3; row <= n; ++row) {
        /* zero next DP */
        for (size_t s = 0; s < states; ++s) dp_next[s] = 0;

        for (size_t s = 0; s < states; ++s) {
            seq_t v = dp_prev[s];
            if (v == 0) continue;
            unsigned char len = adj_len[s];
            for (unsigned char p = 0; p < len; ++p) {
                size_t dst = adj_dst[s][p];
                dp_next[dst] += v;
            }
        }
        /* swap dp arrays */
        for (size_t s = 0; s < states; ++s)
            dp_prev[s] = dp_next[s];
    }

    /* sum all DP states: number of length‑n arrays */
    seq_t total = 0;
    for (size_t s = 0; s < states; ++s)
        total += dp_prev[s];
    return total;
}
