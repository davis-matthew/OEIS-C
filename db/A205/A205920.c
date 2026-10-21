/*
 * A205920  Number of (n+1)X3 0..3 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A205920:   Number of (n+1)×3 0..3 arrays with the number of
 * clockwise edge increases in every 2×2 subblock equal to one,
 * and every 2×2 determinant nonzero.
 * 
 * The sequence is computed by dynamic programming on the
 * adjacency graph of 3‑tuples.  States are the 64 possible rows
 * (3 digits 0..3).  There is an edge i→j iff the two rows
 * satisfy the two subblock constraints (exactly one clockwise
 * increase per subblock and both determinants ≠ 0).  The number of
 * arrays with n+1 rows equals the number of walks of length n
 * in this graph, starting from any state.  The DP computes the
 * number of walks of length n ending in every state and sums
 * them to give a(n).
 * 
 * Complexity:  O(n·64²)  (≈ 4·10⁴ operations per step), easily
 * under the time limit for n ≤ 10 000.
 * 
 * The implementation uses only integer arithmetic, works for
 * arbitrary‐precision seq_t as well as fixed 8‑, 16‑, 32‑,
 * 64‑, or 128‑bit unsigned types.  All intermediate values are
 * accumulated in the same type; no overflow occurs for values
 * that fit in seq_t, which is sufficient for the tests.
 */

#include <stdint.h>
#include <stddef.h>

#define S 64  /* number of states (3 digits 0..3) */

static seq_t base[S][S];
static unsigned char digits[S][3];
static unsigned char base_initialized = 0;

/* Build the adjacency matrix. */
static void init_base(void)
{
    if (base_initialized) return;

    /* decode each state into its 3 digits */
    for (int i = 0; i < S; ++i) {
        uint64_t v = i;
        digits[i][0] = v % 4; v /= 4;
        digits[i][1] = v % 4; v /= 4;
        digits[i][2] = v % 4;
    }

    /* fill adjacency */
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            /* clockwise edge increases in left subblock (cols 0,1) */
            int inc1 = 0;
            inc1 += (digits[i][0] < digits[i][1]) ? 1 : 0;
            inc1 += (digits[i][1] < digits[j][1]) ? 1 : 0;
            inc1 += (digits[j][1] < digits[j][0]) ? 1 : 0;
            inc1 += (digits[j][0] < digits[i][0]) ? 1 : 0;
            if (inc1 != 1) { base[i][j] = 0; continue; }

            /* clockwise edge increases in right subblock (cols 1,2) */
            int inc2 = 0;
            inc2 += (digits[i][1] < digits[i][2]) ? 1 : 0;
            inc2 += (digits[i][2] < digits[j][2]) ? 1 : 0;
            inc2 += (digits[j][2] < digits[j][1]) ? 1 : 0;
            inc2 += (digits[j][1] < digits[i][1]) ? 1 : 0;
            if (inc2 != 1) { base[i][j] = 0; continue; }

            /* 2×2 determinants nonzero */
            int det1 = (int)digits[i][0] * (int)digits[j][1]
                     - (int)digits[i][1] * (int)digits[j][0];
            if (det1 == 0) { base[i][j] = 0; continue; }

            int det2 = (int)digits[i][1] * (int)digits[j][2]
                     - (int)digits[i][2] * (int)digits[j][1];
            if (det2 == 0) { base[i][j] = 0; continue; }

            base[i][j] = 1;
        }
    }

    base_initialized = 1;
}

/* Compute a(n).  n >= 1 */
seq_t a(idx_t n)
{
    if (!base_initialized) init_base();

    seq_t cur[S], next[S];
    /* each state is a possible first row: count 1 */
    for (idx_t i = 0; i < S; ++i) cur[i] = 1;

    /* perform n transitions (edges) */
    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t j = 0; j < S; ++j) next[j] = 0;
        for (idx_t i = 0; i < S; ++i) {
            seq_t ci = cur[i];
            if (ci == 0) continue;
            for (idx_t j = 0; j < S; ++j) {
                if (base[i][j]) next[j] += ci;
            }
        }
        for (idx_t j = 0; j < S; ++j) cur[j] = next[j];
    }

    /* total number of arrays is sum over endings */
    seq_t total = 0;
    for (idx_t j = 0; j < S; ++j) total += cur[j];
    return total;
}
