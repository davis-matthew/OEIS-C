/*
 * A143453  Square array A(n,k) of numbers of length n ternary words with at
 *          least k 0-digits between any other digits (n,k >= 0), read by
 *          antidiagonals.
 * https://oeis.org/A143453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array A(n,k) with the following
 *            recursive definition A(0,k) = 1 A(n,0) = 3^n A(n,k) = 2*n+1 if n
 *            <= k+1 A(n,k) = A(n-1,k) + 2*A(n-k-1,k) otherwise The sequence
 *            a(n) is obtained by reading the array along antidiagonals (n,k)
 *            with n + k = d where d is the diagonal index...
 * Range:     u8 n <= 26, u16 n <= 76, u32 n <= 251, u64 n <= 901,
 *            u128 n <= 3401, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *p) */

/* Static cache for the square array */
static seq_t *A_table = NULL;   /* flattened 2‑D array */
static idx_t A_dim = 0;         /* current table dimension (size = A_dim x A_dim) */

/* Rebuild the array up to dimension new_dim-1 (inclusive) */
static void recompute_table(idx_t new_dim)
{
    if (A_table) free_seq_array(A_table);
    A_dim = new_dim;
    /* allocate contiguous array of seq_t objects */
    A_table = alloc_seq_array(new_dim * new_dim);
    if (!A_table) exit(1); /* out of memory */

    /* First row (n = 0): all 1s */
    for (idx_t k = 0; k < A_dim; ++k) {
        A_table[0 * A_dim + k] = (seq_t)1;
    }

    /* Remaining rows */
    for (idx_t n = 1; n < A_dim; ++n) {
        /* Column k = 0: A(n,0) = 3^n (build incrementally) */
        A_table[n * A_dim + 0] = A_table[(n - 1) * A_dim + 0] * ((seq_t)3);

        for (idx_t k = 1; k < A_dim; ++k) {
            if (n <= k + 1) {
                /* A(n,k) = 2*n + 1 */
                A_table[n * A_dim + k] = ((seq_t)2 * (seq_t)n) + ((seq_t)1);
            } else {
                /* A(n,k) = A(n-1,k) + 2*A(n-k-1,k) */
                seq_t left  = A_table[(n - 1) * A_dim + k];
                seq_t right = A_table[(n - k - 1) * A_dim + k];
                A_table[n * A_dim + k] = left + (right * ((seq_t)2));
            }
        }
    }
}

/* Locate the diagonal (d) such that T(d-1) <= n < T(d), where T(m) = (m+1)(m+2)/2 */
static inline idx_t diagonal_from_index(idx_t n)
{
    long double sq = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t d = (idx_t)((sq - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while (d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    return d;
}

/* OEIS sequence a(n) */
seq_t a(idx_t n)
{
    /* Find (i,j) coordinates in the antidiagonal */
    idx_t d   = diagonal_from_index(n);
    idx_t pos = n - d * (d + 1) / 2;
    idx_t i   = pos;
    idx_t j   = d - pos;

    /* Ensure the table is big enough */
    if (A_dim <= d) {
        recompute_table(d + 1);
    }

    return A_table[i * A_dim + j];
}
