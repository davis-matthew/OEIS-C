/*
 * A207722  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207722: Count of 7×n binary arrays avoiding
   horizontally 000 and 001, and vertically 001 and 100.
   The function uses a transfer‑matrix DP over the 35 valid
   column patterns.  For n = 1 a(1)=35, for n = 2 a(2)=1225.
   For n ≥ 3, a(n) = 35 · Σ_{c} ways_of_length_(n‑1)_ending_with(c)
   where all consecutive column pairs among the first n‑1 columns
   satisfy (col_i | col_{i+1}) == 0x7F. */

#include <stdint.h>
#include <stdbool.h>

static bool init = false;
static uint8_t masks[35];
static int vcnt = 0;
static bool good[35][35];   /* good[i][j] == ((masks[i]|masks[j])==0x7F) */

static void init_tables(void) {
    /* Build the 35 admissible column masks (vertical constraints). */
    for (idx_t m = 0; m < 128; ++m) {
        bool ok = true;
        for (idx_t i = 0; i <= 4; ++i) {
            int sub = (m >> i) & 7;   /* 3‑bit window */
            if (sub == 1 || sub == 4) { /* 001 or 100 forbidden vertically */
                ok = false;
                break;
            }
        }
        if (ok) {
            masks[vcnt++] = (uint8_t)m;
        }
    }
    /* Pre‑compute horizontal pair compatibility. */
    const uint8_t FULL = 0x7F;
    for (int i = 0; i < vcnt; ++i)
        for (int j = 0; j < vcnt; ++j)
            good[i][j] = ((masks[i] | masks[j]) == FULL);
    init = true;
}

seq_t a(idx_t n) {
    if (!init)
        init_tables();

    /* Handle small n directly. */
    if (n == 1)
        return (seq_t)vcnt;
    if (n == 2)
        return (seq_t)vcnt * (seq_t)vcnt;

    /* DP arrays: dp_cur[c] = number of arrays of current length
       ending with column pattern c where all consecutive pairs
       satisfy the OR condition. */
    seq_t dp_cur[35];
    seq_t dp_next[35];
    for (int i = 0; i < vcnt; ++i)
        dp_cur[i] = (seq_t)1;   /* length = 1 */

    idx_t step = 1;           /* current length represented by dp_cur */
    while (step < n - 1) {
        /* Compute dp_next for length step+1 */
        for (int i = 0; i < vcnt; ++i)
            dp_next[i] = (seq_t)0;
        for (int c = 0; c < vcnt; ++c) {
            seq_t val = dp_cur[c];
            if (val == 0)
                continue;
            for (int nxt = 0; nxt < vcnt; ++nxt) {
                if (good[c][nxt])
                    dp_next[nxt] += val;
            }
        }
        for (int i = 0; i < vcnt; ++i)
            dp_cur[i] = dp_next[i];
        ++step;
    }

    /* Sum over all possible final columns (no restriction for the last pair). */
    seq_t total = (seq_t)0;
    for (int c = 0; c < vcnt; ++c) {
        if (dp_cur[c] == 0)
            continue;
        total += dp_cur[c] * (seq_t)vcnt;
    }
    return total;
}
