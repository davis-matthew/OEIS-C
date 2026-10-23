/*
 * A181241  Number of nX6 binary matrices with no 2X2 circuit having pattern
 *          0101 in any orientation.
 * https://oeis.org/A181241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary matrices with no 2×2 checkerboard
 *            (0101) pattern. Computation: dynamic programming over the 64
 *            possible rows
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming over row vectors.  For each row vector r (0..63) we keep the
   number of valid n‑row matrices that end with r.  Two consecutive rows a,b are
   compatible iff they do NOT contain an adjacent pair of columns (j,j+1) with
   the pattern 0 1 / 1 0 (or its rotation).  We precompute the 64×64 adjacency
   matrix once, then iterate n‑1 steps.  a(1) = 64 (any single row). */
#include <stdint.h>
#include <stddef.h>

#define W 6
#define N (1 << W)          /* 64 possible rows */

static seq_t T[N * N];
static int T_built = 0;

/* Build adjacency matrix: T[i][j] == 1 if rows i and j are compatible. */
static void build_T(void)
{
    if (T_built) return;
    for (int a = 0; a < N; ++a) {
        for (int b = 0; b < N; ++b) {
            int ok = 1;
            for (int j = 0; j < W - 1; ++j) {
                int a0 = (a >> j) & 1;
                int a1 = (a >> (j + 1)) & 1;
                int b0 = (b >> j) & 1;
                int b1 = (b >> (j + 1)) & 1;
                if (a0 == b1 && a1 == b0 && a0 != a1) { ok = 0; break; }
            }
            T[a * N + b] = ok ? (seq_t)1 : (seq_t)0;
        }
    }
    T_built = 1;
}

/* Main function: a(n) as defined in OEIS. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined, but safe */
    if (n == 1) return (seq_t)64;     /* first term */

    build_T();                          /* ensure transition matrix ready */

    /* current[row] = number of valid matrices of current height ending with 'row' */
    seq_t current[N];
    seq_t next[N];

    /* base case n = 1: all 64 rows are possible once */
    for (int r = 0; r < N; ++r) current[r] = 1;

    /* iteratively add rows */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero next */
        for (int r = 0; r < N; ++r) next[r] = 0;

        /* transition */
        for (int i = 0; i < N; ++i) {
            if (current[i] != 0) {
                for (int j = 0; j < N; ++j) {
                    if (T[i * N + j]) {
                        next[j] += current[i];
                    }
                }
            }
        }

        /* copy next -> current */
        for (int r = 0; r < N; ++r) current[r] = next[r];
    }

    /* total number of matrices is sum over all ending rows */
    seq_t total = 0;
    for (int r = 0; r < N; ++r) total += current[r];
    return total;
}
