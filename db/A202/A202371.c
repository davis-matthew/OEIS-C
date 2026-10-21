/*
 * A202371  Number of (n+2)X3 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 binary arrays avoiding 001 and 111 in
 *            every row and every column. Computed by dynamic programming over
 *            the last two rows. Recurrence: for every pair of previous rows
 *            (a,b) and every allowed next row r that keeps all three columns
 *            free of 001 or 111, transition to state (b...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 28, u64 n <= 67, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Allowed 3‑bit rows (exclude 001 (1) and 111 (7)) */
static const uint8_t ALLOWED[6] = {0, 2, 3, 4, 5, 6};

seq_t a(idx_t n)
{
    /* Number of rows in the array */
    idx_t rows = n + 2;

    /* Transition data: for each state (a,b) store list of next states (b,r) */
    static int initialized = 0;
    static uint8_t trans_next[36][6]; /* maximum 6 transitions per state */
    static uint8_t trans_cnt[36];

    if (!initialized) {
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                int state = a * 6 + b;
                trans_cnt[state] = 0;
                for (int r = 0; r < 6; ++r) {
                    int ok = 1;
                    for (int col = 0; col < 3 && ok; ++col) {
                        int shift = 2 - col;            /* leftmost bit at shift 2 */
                        int pa = (ALLOWED[a] >> shift) & 1;
                        int pb = (ALLOWED[b] >> shift) & 1;
                        int pr = (ALLOWED[r] >> shift) & 1;
                        if ((pa == 0 && pb == 0 && pr == 1) || /* 001 */
                            (pa == 1 && pb == 1 && pr == 1))  /* 111 */
                            ok = 0;
                    }
                    if (ok) {
                        /* Next state is (b,r) */
                        trans_next[state][trans_cnt[state]] = (uint8_t)(b * 6 + r);
                        trans_cnt[state]++;
                    }
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays: counts of configurations ending with pair (a,b) */
    seq_t cur[36], next[36];
    for (int i = 0; i < 36; ++i) cur[i] = 0;

    /* Initial two rows: any ordered pair of allowed rows */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            cur[a * 6 + b] = 1;

    /* Number of additional rows to add equals n */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 36; ++i) next[i] = 0;
        for (int state = 0; state < 36; ++state) {
            seq_t cnt = cur[state];
            if (!cnt) continue;
            uint8_t *list = trans_next[state];
            uint8_t *end   = list + trans_cnt[state];
            for (uint8_t *p = list; p < end; ++p) {
                int next_state = *p;
                next[next_state] = next[next_state] + cnt;
            }
        }
        for (int i = 0; i < 36; ++i) cur[i] = next[i];
    }

    /* Sum all surviving states */
    seq_t total = 0;
    for (int i = 0; i < 36; ++i) total = total + cur[i];
    return total;
}
