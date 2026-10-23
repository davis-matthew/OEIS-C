/*
 * A125399  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1 for n ≥ 1: a(n) = trace(Aⁿ) where A is the 11×11
 *            adjacency matrix with A[i][j] = 1 if |i−j| ≤ 6 else 0. This
 *            counts the number of circular n‑digit base‑11 numbers whose
 *            adjacent digits differ by at most 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const size_t SZ = 11;          /* number of digits 0…10 */

/* Multiply two SZ×SZ matrices (a·b → out) */
static void mat_mult(const seq_t a[SZ][SZ], const seq_t b[SZ][SZ], seq_t out[SZ][SZ]) {
    for (size_t i = 0; i < SZ; ++i) {
        for (size_t j = 0; j < SZ; ++j) {
            seq_t sum = 0;
            for (size_t k = 0; k < SZ; ++k) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

/* Return A^exp where A is a SZ×SZ matrix */
static void mat_pow(seq_t mat[SZ][SZ], idx_t exp, seq_t res[SZ][SZ]) {
    /* initialize res as identity matrix */
    for (size_t i = 0; i < SZ; ++i) {
        for (size_t j = 0; j < SZ; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }
    seq_t base[SZ][SZ];
    for (size_t i = 0; i < SZ; ++i)
        for (size_t j = 0; j < SZ; ++j)
            base[i][j] = mat[i][j];

    while (exp) {
        if (exp & 1) {
            seq_t temp[SZ][SZ];
            mat_mult(res, base, temp);
            for (size_t i = 0; i < SZ; ++i)
                for (size_t j = 0; j < SZ; ++j)
                    res[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {                     /* avoid last unnecessary multiplication */
            seq_t temp[SZ][SZ];
            mat_mult(base, base, temp);
            for (size_t i = 0; i < SZ; ++i)
                for (size_t j = 0; j < SZ; ++j)
                    base[i][j] = temp[i][j];
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* empty number */

    /* Build adjacency matrix A */
    seq_t A[SZ][SZ];
    for (size_t i = 0; i < SZ; ++i) {
        for (size_t j = 0; j < SZ; ++j) {
            seq_t diff = (i >= j) ? (seq_t)(i - j) : (seq_t)(j - i);
            A[i][j] = (diff <= 6) ? 1 : 0;
        }
    }

    /* Compute Aⁿ */
    seq_t An[SZ][SZ];
    mat_pow(A, n, An);

    /* Sum diagonal entries (trace) */
    seq_t result = 0;
    for (size_t i = 0; i < SZ; ++i)
        result += An[i][i];

    return result;
}
