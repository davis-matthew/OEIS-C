/*
 * A207513  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 23, u64 n <= 57, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A dynamic‑programming recurrence over rows.
   a(1) = 25, a(2) = 625.  For n≥3, the value is obtained by
   counting all n×7 binary arrays that avoid the forbidden
   3×1 blocks 000 and 011 horizontally and the 3×1 blocks
   001 and 111 vertically. */

#include <stdbool.h>   /* for bool, true, false */

#define MAX_H      25
#define MAX_STATES (MAX_H * MAX_H)

seq_t a(idx_t n)
{
    /* --- Pre‑compute the allowed row patterns and transitions --- */
    static bool ready = false;
    static uint8_t rows[MAX_H];           /* allowed 7‑bit row masks */
    static int    trans_len[MAX_STATES];  /* number of transitions for each state */
    static short  trans_next[MAX_STATES][MAX_H]; /* next row index for each state */

    static int H;  /* actual number of allowed rows (≤ MAX_H) */

    if (!ready) {
        /* Build allowed rows */
        H = 0;
        for (int mask = 0; mask < 128; ++mask) {
            bool ok = true;
            for (int pos = 0; pos <= 4; ++pos) {       /* check horizontal 000 & 011 */
                int triple = (mask >> pos) & 7;
                if (triple == 0 || triple == 3) {     /* 000 or 011 */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                rows[H++] = (uint8_t)mask;              /* store mask */
            }
        }

        /* Build transition table: from state (i,j) to next row k */
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < H; ++j) {
                int idx = i * H + j;                    /* state index */
                int len = 0;
                for (int k = 0; k < H; ++k) {
                    bool ok = true;
                    for (int pos = 0; pos < 7; ++pos) {
                        int a = (rows[i] >> pos) & 1;
                        int b = (rows[j] >> pos) & 1;
                        int c = (rows[k] >> pos) & 1;
                        if ((a == 0 && b == 0 && c == 1) ||   /* 001 vertically */
                            (a == 1 && b == 1 && c == 1)) {    /* 111 vertically */
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        trans_next[idx][len++] = (short)k;
                    }
                }
                trans_len[idx] = len;
            }
        }
        ready = true;
    }

    /* --- Base cases --- */
    if (n == 1) return (seq_t)H;
    if (n == 2) return (seq_t)H * (seq_t)H;

    /* --- Dynamic programming over rows --- */
    const int S = H * H;                     /* number of states (i,j) */
    seq_t dp_cur[MAX_STATES];
    seq_t dp_next[MAX_STATES];

    /* For the first two rows: every pair of allowed rows is valid */
    for (int s = 0; s < S; ++s) dp_cur[s] = (seq_t)1;

    /* Process rows 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < S; ++s) dp_next[s] = (seq_t)0;
        for (int s = 0; s < S; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            int len = trans_len[s];
            int j   = s % H;                /* last row of the current state */
            for (int t = 0; t < len; ++t) {
                int k = trans_next[s][t];
                int new_idx = j * H + k;
                dp_next[new_idx] += val;
            }
        }
        /* swap buffers */
        for (int s = 0; s < S; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all ending states */
    seq_t total = (seq_t)0;
    for (int s = 0; s < S; ++s) total += dp_cur[s];
    return total;
}
