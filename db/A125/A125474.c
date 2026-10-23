/*
 * A125474  Number of base 17 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1 where A is the 17×17
 *            adjacency matrix of the graph with vertices 0…16 and an edge
 *            between i and j iff |i−j| ≤ 9 (including the self‑loop). The
 *            trace of A^n counts all circular n‑digit base‑17 numbers whose
 *            adjacent digits differ by at most 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void mat_mul(seq_t out[17][17], const seq_t a[17][17], const seq_t b[17][17])
{
    int i, j, k;
    for (i = 0; i < 17; ++i) {
        for (j = 0; j < 17; ++j) {
            seq_t sum = 0;
            for (k = 0; k < 17; ++k) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

static void copy_mat(seq_t dst[17][17], const seq_t src[17][17])
{
    int i, j;
    for (i = 0; i < 17; ++i) {
        for (j = 0; j < 17; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition of the OEIS sequence */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build the 17×17 adjacency matrix once */
    static seq_t A[17][17];
    static int ready = 0;
    if (!ready) {
        int i, j;
        for (i = 0; i < 17; ++i) {
            for (j = 0; j < 17; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                A[i][j] = (diff <= 9) ? (seq_t)1 : (seq_t)0;
            }
        }
        ready = 1;
    }

    /* Binary exponentiation of the matrix A */
    seq_t R[17][17], M[17][17], T[17][17];
    int i, j;

    /* R := identity matrix */
    for (i = 0; i < 17; ++i) {
        for (j = 0; j < 17; ++j) {
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* M := A */
    copy_mat(M, A);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(T, R, M);   /* R = R * M */
            copy_mat(R, T);
        }
        mat_mul(T, M, M);       /* M = M * M */
        copy_mat(M, T);
        exp >>= 1;
    }

    /* a(n) = trace(R) = sum of diagonal elements of A^n */
    seq_t trace = 0;
    for (i = 0; i < 17; ++i) {
        trace += R[i][i];
    }
    return trace;
}
