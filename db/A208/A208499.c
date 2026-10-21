/*
 * A208499  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208499: Number of n×6 0..1 arrays avoiding horizontally
 * 000 and 001, and vertically 010 and 111.
 *
 * The algorithm uses dynamic programming over pairs of consecutive rows.
 * Let A be the number of legal 6‑bit rows (26).  For each pair (i,j)
 * of legal rows we precompute the list of legal next rows k such that
 * for every column p the vertical triple (i_p, j_p, k_p) is neither
 * 010 nor 111.  The DP state is the pair (i,j); the transition
 * (i,j) → (j,k) is allowed iff k is in the precomputed list.
 * For n=1 the answer is A; for n=2 it is A²; for n≥3 we iterate
 * n‑2 transitions, updating a state vector of size A² (≤676).
 *
 * The implementation is fully static and re‑initializes only once.
 * It works for all provided integer types seq_t and idx_t. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROWS 64
#define MAX_STATES (MAX_ROWS * MAX_ROWS)

seq_t a(idx_t n)
{
    /* Enumerate all legal rows */
    static int row_bits[MAX_ROWS];
    static int idx_of_row[64];
    static int num_rows = 0;
    static bool rows_ready = false;

    if (!rows_ready) {
        for (int m = 0; m < 64; ++m) {
            bool ok = true;
            /* Reject any triple 000 or 001 */
            for (int p = 0; p <= 3; ++p) {
                int a = (m >> p) & 1;
                int b = (m >> (p + 1)) & 1;
                int c = (m >> (p + 2)) & 1;
                if (a == 0 && b == 0 && (c == 0 || c == 1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                idx_of_row[m] = num_rows;
                row_bits[num_rows] = m;
                ++num_rows;
            } else {
                idx_of_row[m] = -1;
            }
        }
        rows_ready = true;
    }

    /* Special cases */
    if (n == 0) return (seq_t)0;                /* unused by OEIS */
    if (n == 1) return (seq_t)num_rows;
    if (n == 2) return (seq_t)num_rows * (seq_t)num_rows;

    /* Transition tables (computed once) */
    static int next_list[MAX_STATES][MAX_ROWS];
    static unsigned char next_len[MAX_STATES];
    static bool trans_ready = false;

    if (!trans_ready) {
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_rows; ++j) {
                unsigned char len = 0;
                int bm = row_bits[i];
                int bm2 = row_bits[j];
                for (int k = 0; k < num_rows; ++k) {
                    bool ok = true;
                    int bm3 = row_bits[k];
                    for (int p = 0; p < 6; ++p) {
                        int a = (bm >> p) & 1;
                        int b = (bm2 >> p) & 1;
                        int c = (bm3 >> p) & 1;
                        if ((a == 0 && b == 1 && c == 0) ||
                            (a == 1 && b == 1 && c == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        next_list[i * num_rows + j][len++] = k;
                    }
                }
                next_len[i * num_rows + j] = len;
            }
        }
        trans_ready = true;
    }

    /* DP arrays */
    static seq_t dp[MAX_STATES];
    static seq_t ndp[MAX_STATES];
    size_t states = (size_t)num_rows * (size_t)num_rows;

    /* Initialize for length 2: every pair is possible */
    memset(dp, 0, sizeof(dp));
    for (size_t s = 0; s < states; ++s) dp[s] = (seq_t)1;

    /* Iterate from length 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (size_t s = 0; s < states; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            unsigned char len = next_len[s];
            for (unsigned char t = 0; t < len; ++t) {
                int k = next_list[s][t];
                size_t ns = (s % num_rows) * num_rows + k;   /* new state (j,k) */
                ndp[ns] += val;
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over all states */
    seq_t total = 0;
    for (size_t s = 0; s < states; ++s) {
        total += dp[s];
    }
    return total;
}
