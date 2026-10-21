/*
 * A085484  Symmetric square array, read by antidiagonals: T(k, k) = T(0, k +
 *          1) = Sum_{m = 0..k} C(k, m)*T(m, k - m) for k >= 0; T(0, 0) = 1;
 *          T(n, k) = T(n - 1, k) + T(n, k - 1) for n, k >= 1.
 * https://oeis.org/A085484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Symmetric square array A085484.
   T[0][0] = 1.
   For k >= 1:
     T[0][k] = sum_{m=0..k-1} C(k-1,m) * T[m][k-1-m].
     T[k][0] = T[0][k].
   For i>0, j>0:
     T[i][j] = T[i-1][j] + T[i][j-1].
   a(n) is obtained by reading T along antidiagonals:
     Let s be the largest integer with s*(s+1)/2 <= n.
     Let i = n - s*(s+1)/2, j = s - i; then a(n) = T[i][j].
   All arithmetic is exact on the type seq_t. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find diagonal index s such that s*(s+1)/2 <= n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* allocate symmetric matrix of size (s+1) x (s+1) */
    idx_t sz = s + 1;
    size_t size = (size_t)sz * (size_t)sz;
    seq_t *T = (seq_t *)calloc(size, sizeof(seq_t));
    if (!T) {
        return (seq_t)0;   /* allocation failure – undefined but safe */
    }

    /* helper to compute linear index */
    #define IDX(i,j) ((size_t)(i) * sz + (size_t)(j))

    /* base case */
    T[IDX(0,0)] = (seq_t)1;

    /* build table iteratively */
    for (idx_t curr = 1; curr <= s; ++curr) {
        /* first row entry T[0][curr] via binomial sum */
        seq_t row_val = (seq_t)0;
        idx_t k = curr - 1;          /* parameter of the sum */
        for (idx_t m = 0; m <= k; ++m) {
            seq_t c = binomial(k, m);
            seq_t t = T[IDX(m, k - m)];
            seq_t prod = c * t;
            row_val += prod;
        }
        T[IDX(0, curr)] = row_val;
        T[IDX(curr, 0)] = row_val;   /* symmetry */

        /* interior entries on diagonal curr (sum = curr) */
        for (idx_t i = 1; i < curr; ++i) {
            idx_t j = curr - i;
            T[IDX(i, j)] = T[IDX(i - 1, j)] + T[IDX(i, j - 1)];
        }
    }

    /* map n to coordinates (i,j) on antidiagonal s */
    idx_t i = n - s * (s + 1) / 2;
    idx_t j = s - i;
    seq_t result = T[IDX(i, j)];

    free(T);
    #undef IDX
    return result;
}
