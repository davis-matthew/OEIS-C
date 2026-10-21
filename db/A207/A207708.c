/*
 * A207708  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n 0..1 arrays avoiding * horizontally 0 0 0
 *            and 0 1 0; vertically 0 0 1 and 1 1 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static initialization performed once */
    static bool ready = false;
    /* constants */
    enum { MAX_BITS = 7 };
    enum { MAX_COLS = 1 << MAX_BITS };           /* 128 */
    enum { MAX_STATE = MAX_COLS * MAX_COLS };     /* 16384 */

    /* data for admissible columns */
    static int col_mask[MAX_COLS];
    static int col_index[MAX_COLS];
    static int col_count;

    /* data for transitions allowed by horizontal rule */
    static int allowed_next[MAX_COLS][MAX_COLS];
    static int allowed_cnt[MAX_COLS];

    /* DP arrays */
    static seq_t dp[MAX_STATE];
    static seq_t next_dp[MAX_STATE];

    /* initialise data once */
    if (!ready) {
        /* mark all columns as not admissible */
        for (int i = 0; i < MAX_COLS; ++i) col_index[i] = -1;
        col_count = 0;

        /* find all vertical‑admissible 7‑bit columns */
        for (int w = 0; w < MAX_COLS; ++w) {
            bool ok = true;
            for (int i = 0; i <= MAX_BITS - 3; ++i) {
                int trip = (w >> i) & 7;               /* 3 consecutive bits */
                if (trip == 1 || trip == 6) {           /* 001 or 110 */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                col_mask[col_count] = w;
                col_index[w] = col_count;
                ++col_count;
            }
        }

        /* pre‑compute for each column p the set of columns r that satisfy the
         * horizontal rule.  For a given row i:
         *   if the bit in column p is 0, the bit in r must be 1.
         *   if the bit in column p is 1, the bit in r may be 0 or 1.  */
        const int MASK_ALL = (1 << MAX_BITS) - 1;
        for (int p = 0; p < col_count; ++p) {
            int p_bits = col_mask[p];
            int cnt = 0;
            for (int r = 0; r < col_count; ++r) {
                int r_bits = col_mask[r];
                /* forbidden: a row where p = 0 and r = 0 */
                if (((~p_bits) & (~r_bits) & MASK_ALL) == 0) {
                    allowed_next[p][cnt++] = r;
                }
            }
            allowed_cnt[p] = cnt;
        }

        ready = true;
    }

    /* the sequence has offset 1 */
    if (n == 0) return 0;                       /* undefined, safe guard */
    if (n == 1) return (seq_t)col_count;        /* only one column */
    if (n == 2) return (seq_t)col_count * (seq_t)col_count; /* any pair */

    /* dynamic programming for width >= 3 */
    const int state_count = col_count * col_count;

    /* initial state for width 2: each pair (p,q) is possible exactly once */
    for (int i = 0; i < state_count; ++i) dp[i] = (seq_t)1;

    /* extend one column at a time */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next_dp */
        for (int i = 0; i < state_count; ++i) next_dp[i] = 0;

        /* for each state (p,q) add transitions to (q,r) */
        for (int p = 0; p < col_count; ++p) {
            int p_offset = p * col_count;          /* index of dp[p,*] block */
            for (int q = 0; q < col_count; ++q) {
                seq_t val = dp[p_offset + q];
                if (val == 0) continue;

                /* for this p, iterate over all admissible r */
                int cnt = allowed_cnt[p];
                for (int t = 0; t < cnt; ++t) {
                    int r = allowed_next[p][t];
                    int new_index = q * col_count + r;
                    next_dp[new_index] = next_dp[new_index] + val;
                }
            }
        }

        /* copy back for next iteration */
        for (int i = 0; i < state_count; ++i) dp[i] = next_dp[i];
    }

    /* sum all states to obtain the total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < state_count; ++i) total = total + dp[i];
    return total;
}
