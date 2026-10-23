/*
 * A125331  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 21×21 adjacency matrix * with
 *            A[i][j] = 1 if |i−j| ≤ 3, otherwise 0. * Special case: a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */

#define BASE 21

/* Multiply two 21×21 matrices: R = M * N */
static void mat_mult(seq_t R[BASE][BASE],
                     const seq_t M[BASE][BASE],
                     const seq_t N[BASE][BASE])
{
    seq_t temp[BASE][BASE];
    for (size_t i = 0; i < BASE; ++i) {
        for (size_t j = 0; j < BASE; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < BASE; ++k) {
                sum += M[i][k] * N[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy temp to R */
    for (size_t i = 0; i < BASE; ++i)
        for (size_t j = 0; j < BASE; ++j)
            R[i][j] = temp[i][j];
}

/* Build the 21×21 adjacency matrix */
static void build_adjaceny(seq_t A[BASE][BASE])
{
    for (size_t i = 0; i < BASE; ++i) {
        for (size_t j = 0; j < BASE; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= (idx_t)3) ? (seq_t)1 : (seq_t)0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;      /* a(0) = 1 */

    /* Result matrix (identity) and current power (adjacency) */
    seq_t result[BASE][BASE];
    seq_t cur[BASE][BASE];
    seq_t temp[BASE][BASE];

    /* Initialize identity matrix */
    for (size_t i = 0; i < BASE; ++i)
        for (size_t j = 0; j < BASE; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Build adjacency matrix */
    build_adjaceny(cur);

    /* Fast exponentiation: result = result * cur^(n) */
    idx_t power = n;
    while (power > (idx_t)0) {
        if (power & (idx_t)1) {
            mat_mult(temp, result, cur);
            for (size_t i = 0; i < BASE; ++i)
                for (size_t j = 0; j < BASE; ++j)
                    result[i][j] = temp[i][j];
        }
        power >>= (idx_t)1;
        if (power == (idx_t)0) break;
        mat_mult(temp, cur, cur);
        for (size_t i = 0; i < BASE; ++i)
            for (size_t j = 0; j < BASE; ++j)
                cur[i][j] = temp[i][j];
    }

    /* Compute trace of result */
    seq_t trace = (seq_t)0;
    for (size_t i = 0; i < BASE; ++i)
        trace += result[i][i];

    return trace;
}
