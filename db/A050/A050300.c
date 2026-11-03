/*
 * A050300  Matrix 6th power of partition triangle A008284.
 * https://oeis.org/A050300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element number n in the row‑wise flattening of the 6th
 *            power of the partition triangle A008284 (lower‑triangular matrix
 *            P where P[i][j] is the number of partitions of i into exactly j
 *            parts)
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Allocate a zeroed square matrix of size (R+1)×(R+1). The indices
   0..R are used; 0‑th row/col are unused. */
static seq_t** alloc_matrix(idx_t R)
{
    seq_t **M = (seq_t**)malloc((R + 1) * sizeof(seq_t*));
    for (idx_t i = 0; i <= R; ++i)
        M[i] = mem_new(seq_t, R + 1);
    return M;
}

/* Create the partition triangle P of size R.  P[i][j] is the number of
   partitions of i into exactly j parts. */
static seq_t** create_partition_matrix(idx_t R)
{
    seq_t **P = alloc_matrix(R);
    P[0][0] = 1;                /* empty partition */

    for (idx_t i = 1; i <= R; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            if (i == j || j == 1)
                P[i][j] = 1; /* one part or all parts used */
            else
                P[i][j] = P[i-1][j-1] + P[i-j][j];
        }
    }
    return P;
}

/* Free a matrix allocated by alloc_matrix. */
static void free_matrix(seq_t **M, idx_t R)
{
    for (idx_t i = 0; i <= R; ++i)
        mem_free(M[i]);
    free(M);
}

/* Multiply two lower‑triangular matrices A and B of size R.  The result
   is a new matrix C = A·B. */
static seq_t** multiply(const seq_t **A, const seq_t **B, idx_t R)
{
    seq_t **C = alloc_matrix(R);

    for (idx_t i = 1; i <= R; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            seq_t sum = 0;
            for (idx_t k = j; k <= i; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
    return C;
}

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* Return the a(n) term.  All work is done locally); no static state is
   required.  The function is deterministic and works for any n ≥ 1. */
seq_t a(idx_t n)
{
    /* Determine the minimal R such that R(R+1)/2 ≥ n. */
    idx_t R = 1;
    while ((R * (R + 1)) / 2 < n)
        ++R;

    /* Build the base partition triangle P. */
    seq_t **P = create_partition_matrix(R);

    /* Compute P^2, P^4, and finally P^6 via squaring. */
    seq_t **P2 = multiply((const seq_t**)P, (const seq_t**)P, R);
    seq_t **P4 = multiply((const seq_t**)P2, (const seq_t**)P2, R);
    seq_t **P6 = multiply((const seq_t**)P4, (const seq_t**)P2, R);

    /* Flatten row‑wise and pick the n‑th element (1‑based). */
    idx_t pos = 0;
    seq_t result = 0;
    for (idx_t i = 1; i <= R && result == 0; ++i) {
        for (idx_t j = 1; j <= i; ++j) {
            ++pos;
            if (pos == n) {
                result = P6[i][j];
                break;
            }
        }
    }

    /* Free all allocated matrices. */
    free_matrix(P, R);
    free_matrix(P2, R);
    free_matrix(P4, R);
    free_matrix(P6, R);

    return result;
}
