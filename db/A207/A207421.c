/*
 * A207421  Number of nX3 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* O(n) dynamic‑programming solution for A207421.
   The recurrence is not given explicitly; we build all
   allowed states explicitly.
   Initial values: a(1) = 6, a(2) = 36, a(3) = 114 ...   */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special cases for n = 0 (not part of sequence) */
    if (n == 0) return 0;

    /* If n == 1 or 2 we can answer directly */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    /* 6 possible row patterns that avoid horizontal
       forbidden triples 000 and 010. Their binary masks:
       001, 011, 100, 101, 110, 111 -> decimal 1,3,4,5,6,7 */
    static const int row_bits[6] = {1, 3, 4, 5, 6, 7};

    /* Build transition table:
       allowed[i][j][k] == true iff the triple of consecutive rows
       i (row r-2), j (row r-1), k (row r) satisfies
         - row k avoids 000/010 (already true by construction)
         - for each column c = 0..2,
           the vertical triple (bit_i, bit_j, bit_k) != 001 and != 010
    */
    static bool allowed[6][6][6] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 6; ++i) {
            int bm_i = row_bits[i];
            for (int j = 0; j < 6; ++j) {
                int bm_j = row_bits[j];
                for (int k = 0; k < 6; ++k) {
                    int bm_k = row_bits[k];
                    bool ok = true;
                    for (int c = 0; c < 3; ++c) {
                        int a = (bm_i >> c) & 1;
                        int b = (bm_j >> c) & 1;
                        int cbit = (bm_k >> c) & 1;
                        if ((a == 0 && b == 0 && cbit == 1) ||
                            (a == 0 && b == 1 && cbit == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    allowed[i][j][k] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP arrays: dp[state] where state = i*6 + j represents
       the last two rows (i: row r-1, j: row r).  The value is the
       number of valid sequences ending in that state.        */
    seq_t dp[36] = {0}, ndp[36] = {0};

    /* For length 2 each of the 36 states is reachable exactly once. */
    for (int s = 0; s < 36; ++s) dp[s] = 1;

    /* Build rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* reset ndp */
        for (int s = 0; s < 36; ++s) ndp[s] = 0;
        for (int s = 0; s < 36; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int i = s / 6;   /* row r-2 */
            int j = s % 6;   /* row r-1 */
            for (int k = 0; k < 6; ++k) {          /* candidate row r */
                if (!allowed[i][j][k]) continue;
                int new_state = j * 6 + k;          /* rows r-1 and r */
                ndp[new_state] += val;
            }
        }
        /* move to next step */
        for (int s = 0; s < 36; ++s) dp[s] = ndp[s];
    }

    /* Sum all states for the total number of sequences of length n */
    seq_t total = 0;
    for (int s = 0; s < 36; ++s) total += dp[s];
    return total;
}
