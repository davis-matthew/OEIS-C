/*
 * A191898  Symmetric square array read by antidiagonals: T(n,1)=1, T(1,k)=1,
 *          T(n,k) = -Sum_{i=1..k-1} T(n-i,k) for n >= k, -Sum_{i=1..n-1}
 *          T(k-i,n) for n < k.
 * https://oeis.org/A191898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the symmetric array T(n,k) defined
 *            by T(n,1) = T(1,n) = 1, T(n,k) = -∑_{i=1}^{k-1} T(n-i,k) for n ≥
 *            k. The matrix is symmetric (T(k,n)=T(n,k)), and the sequence
 *            a(n) is the result of reading T along anti‑diagonals. Initial
 *            values: a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=-1,
 * Range:     i8 n <= 9968, i16 n <= 9968, i32 n <= 9968, i64 n <= 9968,
 *            i128 n <= 9968, big any n
 * Helpers:   getT_linear
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/getT_linear.h"

/* Cache for the upper‑triangular matrix T */
static seq_t *T_data = NULL;
static idx_t width = 0;                 /* current size (max index used) */

/* Access T(i,j) using the global matrix */
static inline seq_t getT_global(idx_t i, idx_t j)
{
    if (i < j) { idx_t t = i; i = j; j = t; }
    return T_data[i * (width + 1) + j];
}

/* Ensure the matrix is at least of size need × need */
static void ensure_size(idx_t need)
{
    if (need <= width) return;

    idx_t new_width = need;
    seq_t *new_data = (seq_t*)malloc((new_width + 1) * (new_width + 1) * sizeof(seq_t));
    if (!new_data) exit(1);  /* out of memory */

    /* initialise new matrix to 0 */
    for (idx_t k = 0; k < (new_width + 1) * (new_width + 1); ++k)
        new_data[k] = 0;

    /* copy old values */
    if (T_data) {
        for (idx_t i = 1; i <= width; ++i) {
            for (idx_t j = 1; j <= width; ++j) {
                new_data[i * (new_width + 1) + j] = T_data[i * (width + 1) + j];
            }
        }
        free(T_data);
    }

    /* compute missing rows */
    idx_t stride = new_width + 1;
    for (idx_t i = width + 1; i <= new_width; ++i)
        for (idx_t j = 1; j <= i; ++j) {
            if (j == 1) {
                new_data[i * stride + 1] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t t = 1; t < j; ++t)
                    sum -= getT_linear(new_data, stride, i - t, j);
                new_data[i * stride + j] = sum;
            }
        }

    T_data = new_data;
    width  = new_width;
}

/* Convert an index n (1‑based) to the anti‑diagonal coordinates (i,j) */
static void antidiag_idx(idx_t n, idx_t *i, idx_t *j)
{
    idx_t d = 1;
    /* Find the smallest d with d(d+1)/2 >= n */
    while (((idx_t)((unsigned __int128)(d + 1) * d / 2)) < n) ++d;
    idx_t start = ((idx_t)((unsigned __int128)(d - 1) * d / 2)) + 1;
    idx_t pos   = n - start + 1;   /* position on this diagonal, 1‑based */

    *i = pos;
    *j = d + 1 - pos;
}

/* OEIS sequence A191898 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined; return 0 */

    idx_t i, j;
    antidiag_idx(n, &i, &j);
    ensure_size(i > j ? i : j);
    return getT_global(i, j);
}
