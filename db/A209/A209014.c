/*
 * A209014  Number of n X 3 0..3 arrays with no element equal the average of
 *          immediate neighbors vertically above, diagonally above and left,
 *          and horizontally left of it.
 * https://oeis.org/A209014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×3 arrays with entries 0..3 satisfying that no
 *            element equals the average of its existing neighbors: vertically
 *            above, diagonally above–left, and horizontally left
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   dig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts all 3‑digit base‑4 rows that satisfy internal constraints.
   The transition between two consecutive rows a→b is allowed iff
   all cell‑wise restrictions involving the three (or fewer) neighbors
   are satisfied. */

#include <stdint.h>
#include "../utils/dig.h"

static uint8_t M[64][64];
static int M_ready = 0;

/* check that a row r satisfies internal left‑neighbour constraints
   (first row of the array) */
static inline int row_ok(uint8_t r) {
    return !(dig(r,1) == dig(r,0) || dig(r,2) == dig(r,1));
}

/* compute transition matrix once */
static void init_matrix(void) {
    if (M_ready) return;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            int ok = 1;
            /* column 0 : need b0 != a0 */
            if (dig(b,0) == dig(a,0)) ok = 0;
            /* column 1 : average of a1, a0, b0 (three existing neighbors) */
            if (ok) {
                int sum = dig(b,0) + dig(a,1) + dig(a,0);
                if (sum % 3 == 0 && dig(b,1) == sum / 3) ok = 0;
            }
            /* column 2 : average of a2, a1, b1 */
            if (ok) {
                int sum = dig(b,1) + dig(a,2) + dig(a,1);
                if (sum % 3 == 0 && dig(b,2) == sum / 3) ok = 0;
            }
            M[a][b] = (uint8_t)ok;
        }
    }
    M_ready = 1;
}

seq_t a(idx_t n) {
    static int initialized = 0;
    if (!initialized) {
        init_matrix();
        initialized = 1;
    }
    /* dp[i] = number of ways to reach current row i at this step */
    seq_t dp[64];
    seq_t next[64];
    for (int i = 0; i < 64; ++i) dp[i] = (seq_t)0;
    /* first row initialization */
    for (int i = 0; i < 64; ++i) {
        if (row_ok((uint8_t)i)) dp[i] = (seq_t)1;
    }
    if (n == 1) {
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 64; ++i) sum += dp[i];
        return sum;          /* should be 36 */
    }
    /* iterate rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < 64; ++i) next[i] = (seq_t)0;
        for (int a = 0; a < 64; ++a) {
            seq_t val = dp[a];
            if (val == (seq_t)0) continue;
            for (int b = 0; b < 64; ++b) {
                if (M[a][b]) next[b] += val;
            }
        }
        /* copy next to dp */
        for (int i = 0; i < 64; ++i) dp[i] = next[i];
    }
    /* sum all ways for last row */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 64; ++i) sum += dp[i];
    return sum;
}
