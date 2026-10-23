/*
 * A124952  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of closed walks of length n on a 24‑node graph *
 *            with edges between nodes whose labels differ by at most 2
 *            (including loops). * Computed by repeatedly multiplying the
 *            adjacency matrix by itself, * but each multiplication uses only
 *            additions (no large‑number multiplications), * so al...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int DIM = 24;

static inline void mult_identity_plus_adj(seq_t cur[DIM][DIM], seq_t tmp[DIM][DIM])
{
    /* Multiply cur by the adjacency matrix A (entries 0 or 1).  The
     * result is stored in tmp.  A[j][k] == 1 iff |j - k| <= 2,
     * i.e. j is in the range [k-2, k+2] (clamped to 0..23). */
    for (int i = 0; i < DIM; ++i) {
        for (int k = 0; k < DIM; ++k) {
            seq_t sum = 0;
            int jmin = k - 2; if (jmin < 0) jmin = 0;
            int jmax = k + 2; if (jmax > DIM - 1) jmax = DIM - 1;
            for (int j = jmin; j <= jmax; ++j) {
                sum += cur[i][j];
            }
            tmp[i][k] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* cur holds the current power of the adjacency matrix.
     * Start with the identity matrix (A^0). */
    seq_t cur[DIM][DIM];
    seq_t tmp[DIM][DIM];
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cur[i][j] = 0;
        }
        cur[i][i] = 1;
    }

    /* Multiply by A exactly n times: cur = A^n. */
    for (idx_t step = 0; step < n; ++step) {
        mult_identity_plus_adj(cur, tmp);
        /* copy tmp -> cur */
        for (int i = 0; i < DIM; ++i)
            for (int j = 0; j < DIM; ++j)
                cur[i][j] = tmp[i][j];
    }

    /* Compute the trace of the resulting matrix: a(n) = trace(A^n). */
    seq_t result = 0;
    for (int i = 0; i < DIM; ++i)
        result += cur[i][i];

    return result;
}
