/*
 * A207662  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 9, a(2) = 81.
 * For n >= 3 the sequence satisfies the recurrence obtained by
 * a transfer‑matrix DP on pairs of consecutive columns.
 * Let C = {0000,0101,0110,0111,1010,1011,1101,1110,1111}
 * (9 admissible 4‑bit columns).
 * A pair (p,q) of columns is a state.
 * From state (p,q) we may go to (q,r) iff for every row
 * the triple (p_row, q_row, r_row) is not all 0 or all 1.
 * The number of n‑column arrays is the sum of all state counts
 * after processing n columns.
 *
 * The code below uses an O(n) algorithm with
 * 81 states and 9 possible next columns for each state.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precomputed data for the 9 admissible column patterns */
    static const int col_bits[9] = {0, 5, 6, 7, 10, 11, 13, 14, 15};
    static int idx_of[16];
    static uint16_t next_mask[9][9];
    static int initialized = 0;

    /* initialise tables once */
    if (!initialized) {
        for (int i = 0; i < 16; ++i) idx_of[i] = -1;
        for (int i = 0; i < 9; ++i) idx_of[col_bits[i]] = i;

        /* build transition masks */
        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                uint16_t mask = 0;
                for (int c = 0; c < 9; ++c) {
                    int ok = 1;
                    for (int r = 0; r < 4; ++r) {
                        int b2 = (col_bits[a] >> r) & 1;
                        int b1 = (col_bits[b] >> r) & 1;
                        int b0 = (col_bits[c] >> r) & 1;
                        if (b2 == b1 && b1 == b0) { ok = 0; break; }
                    }
                    if (ok) mask |= (uint16_t)(1 << c);
                }
                next_mask[a][b] = mask;
            }
        }
        initialized = 1;
    }

    /* trivial cases */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    /* DP over pairs of columns.
       dp_cur[81] holds counts for the current length,
       dp_next[81] holds counts for the next length. */
    static seq_t dp0[81], dp1[81];
    seq_t *dp_cur = dp0, *dp_next = dp1;

    /* start with all possible pairs for length 2 */
    for (int i = 0; i < 81; ++i) dp_cur[i] = (seq_t)1;
    seq_t total = (seq_t)81;   /* a(2) */

    for (idx_t len = 3; len <= n; ++len) {
        /* clear next DP */
        for (int i = 0; i < 81; ++i) dp_next[i] = (seq_t)0;

        /* transition */
        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 9; ++b) {
                seq_t val = dp_cur[a * 9 + b];
                if (!val) continue;
                uint16_t mask = next_mask[a][b];
                for (int c = 0; c < 9; ++c) {
                    if (mask & (1u << c))
                        dp_next[b * 9 + c] += val;
                }
            }
        }

        /* compute total for this length */
        total = 0;
        for (int i = 0; i < 81; ++i) total += dp_next[i];

        /* swap DP arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    return total;
}
