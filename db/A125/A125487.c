/*
 * A125487  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n≥1, where A is the 30×30 adjacency matrix
 *            with A[i][j] = 1 if |i – j| ≤ 9, else 0. For n = 0, a(0) = 1
 *            (the empty circular number)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  NOTE:  This implementation works for any integer types
    supplied by the build system:  idx_t (unsigned 64‑bit) and
    seq_t (an unsigned integer or arbitrary‑precision class).  */

#include <stdbool.h>
#include <stdlib.h>

#define DIM 30   /* number of digits in base 30 */

static void mat_mul(const seq_t A[DIM][DIM],
                    const seq_t B[DIM][DIM],
                    seq_t C[DIM][DIM])
{
    /* C = A * B  (matrix multiplication) */
    int r, c, k;
    for (r = 0; r < DIM; ++r) {
        for (c = 0; c < DIM; ++c) {
            seq_t sum = (seq_t)0;
            for (k = 0; k < DIM; ++k) {
                sum += A[r][k] * B[k][c];
            }
            C[r][c] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    /* Empty number */
    if (n == 0) {
        return (seq_t)1;
    }

    /* adjacency matrix A */
    seq_t A[DIM][DIM];
    int i, j;
    for (i = 0; i < DIM; ++i) {
        for (j = 0; j < DIM; ++j) {
            A[i][j] = (abs(i - j) <= 9) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* res = A^n   (computed by binary exponentiation) */
    seq_t res[DIM][DIM];
    seq_t tmp[DIM][DIM];
    /* res ← identity matrix */
    for (i = 0; i < DIM; ++i) {
        for (j = 0; j < DIM; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, A, tmp);
            for (i = 0; i < DIM; ++i) {
                for (j = 0; j < DIM; ++j) {
                    res[i][j] = tmp[i][j];
                }
            }
        }
        mat_mul(A, A, tmp);
        for (i = 0; i < DIM; ++i) {
            for (j = 0; j < DIM; ++j) {
                A[i][j] = tmp[i][j];
            }
        }
        exp >>= 1;
    }

    /* trace of res is the answer */
    seq_t ans = (seq_t)0;
    for (i = 0; i < DIM; ++i) {
        ans += res[i][i];
    }
    return ans;
}
