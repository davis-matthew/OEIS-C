/*
 * A125387  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n==0; otherwise a(n) = trace(A^n) where * A[i][j] =
 *            1 if |i-j| <= 5 (within base‑24) else 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>   /* for memset */

/* Forward declaration needed for C++ compatibility */
seq_t a(idx_t n);

/* Multiply two 24×24 matrices over seq_t: out = a * b. */
static void matmul(seq_t out[24][24],
                   const seq_t a_mat[24][24],
                   const seq_t b_mat[24][24])
{
    int i, j, k;
    for (i = 0; i < 24; ++i) {
        for (j = 0; j < 24; ++j) {
            seq_t sum = 0;
            for (k = 0; k < 24; ++k) {
                sum += a_mat[i][k] * b_mat[k][j];
            }
            out[i][j] = sum;
        }
    }
}

/* Return a(n). */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Static adjacency matrix A. */
    static seq_t A[24][24];
    static int init = 0;

    if (!init) {
        int i, j;
        for (i = 0; i < 24; ++i) {
            for (j = 0; j < 24; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                A[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        init = 1;
    }

    /* Result matrix = I. */
    seq_t R[24][24];
    int i, j;
    for (i = 0; i < 24; ++i) {
        for (j = 0; j < 24; ++j) {
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Power matrix = A. */
    seq_t P[24][24];
    for (i = 0; i < 24; ++i) {
        for (j = 0; j < 24; ++j) {
            P[i][j] = A[i][j];
        }
    }

    idx_t e = n;
    seq_t Temp[24][24];

    while (e > 0) {
        if (e & 1) {
            matmul(Temp, R, P);
            for (i = 0; i < 24; ++i)
                for (j = 0; j < 24; ++j)
                    R[i][j] = Temp[i][j];
        }
        e >>= 1;
        if (e == 0) break;
        matmul(Temp, P, P);
        for (i = 0; i < 24; ++i)
            for (j = 0; j < 24; ++j)
                P[i][j] = Temp[i][j];
    }

    /* Compute trace of R (= A^n). */
    seq_t sum = 0;
    for (i = 0; i < 24; ++i) {
        sum += R[i][i];
    }

    return sum;
}
