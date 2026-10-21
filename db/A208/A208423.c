/*
 * A208423  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A208423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Code uses a transfer‑matrix DP to compute a(n) for 6×n binary arrays
    avoiding the forbidden patterns 000,001 horizontally and 000,011 vertically. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- Pre‑compute admissible columns and transitions
       (executed only once, independent of n) ---------- */
    static int initialized = 0;
    static int Ncol;                    /* number of admissible columns   */
    static int cols[64];                /* index → 6‑bit column value     */
    static int col_to_idx[64];          /* 6‑bit value → index (-1 if not)*/
    static int state_count;             /* Ncol * Ncol                    */
    static int trans[64*64][64];        /* outgoing transitions           */
    static int trans_len[64*64];        /* length of trans[s]             */

    /* DP storage (allocated after state_count is known) */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static size_t dp_size = 0;

    if (!initialized) {
        /* 1. Find all columns that satisfy the vertical restrictions */
        memset(col_to_idx, -1, sizeof(col_to_idx));
        Ncol = 0;
        for (int col = 0; col < 64; ++col) {
            int ok = 1;
            for (int i = 0; i <= 3; ++i) {   /* 4 possible triples in a 6‑bit column */
                int b0 = (col >> i)     & 1;
                int b1 = (col >> (i+1)) & 1;
                int b2 = (col >> (i+2)) & 1;
                if ((b0==0 && b1==0 && b2==0) || (b0==0 && b1==1 && b2==1)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                cols[Ncol] = col;
                col_to_idx[col] = Ncol;
                ++Ncol;
            }
        }

        /* 2. Build transition graph (state = pair of consecutive columns). */
        state_count = Ncol * Ncol;
        for (int a = 0; a < Ncol; ++a) {
            for (int b = 0; b < Ncol; ++b) {
                int s = a * Ncol + b;
                trans_len[s] = 0;
                for (int c = 0; c < Ncol; ++c) {
                    int ok = 1;
                    for (int r = 0; r < 6; ++r) {
                        int bit_a = (cols[a] >> r) & 1;
                        int bit_b = (cols[b] >> r) & 1;
                        int bit_c = (cols[c] >> r) & 1;
                        if (bit_a==0 && bit_b==0 && bit_c==0) { ok = 0; break; }
                        if (bit_a==0 && bit_b==0 && bit_c==1) { ok = 0; break; }
                    }
                    if (ok) {
                        int next_state = b * Ncol + c;
                        trans[s][trans_len[s]++] = next_state;
                    }
                }
            }
        }

        /* Allocate DP buffers once */
        dp_size = (size_t)state_count;
        dp_cur = (seq_t*)calloc(dp_size, sizeof(seq_t));
        dp_next = (seq_t*)calloc(dp_size, sizeof(seq_t));

        initialized = 1;
    }

    /* ---------- Handle small n directly ---------- */
    if (n == 1) return (seq_t)Ncol;
    if (n == 2) return (seq_t)Ncol * (seq_t)Ncol;

    /* ---------- Dynamic programming for n ≥ 3 ---------- */
    /* Initialise dp_cur for length 2 : each state count = 1 */
    for (size_t i = 0; i < dp_size; ++i) dp_cur[i] = 1;

    /* Iterate columns 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next buffer */
        for (size_t i = 0; i < dp_size; ++i) dp_next[i] = 0;

        for (size_t s = 0; s < dp_size; ++s) {
            if (dp_cur[s] == 0) continue;
            int d = trans_len[s];
            for (int k = 0; k < d; ++k) {
                int ns = trans[s][k];
                dp_next[ns] += dp_cur[s];
            }
        }
        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over all end states to obtain total number of arrays */
    seq_t result = 0;
    for (size_t i = 0; i < dp_size; ++i) result += dp_cur[i];
    return result;
}
