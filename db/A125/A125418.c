/*
 * A125418  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| <= 6 in a base‑30
 *            digit set, else 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define BASE 30

/* Multiply two BASE×BASE matrices X and Y over seq_t, store result in R. */
static void mat_mul(seq_t R[BASE][BASE], const seq_t X[BASE][BASE],
                    const seq_t Y[BASE][BASE])
{
    seq_t sum;
    for (size_t i = 0; i < BASE; ++i) {
        for (size_t j = 0; j < BASE; ++j) {
            sum = (seq_t)0;
            for (size_t k = 0; k < BASE; ++k) {
                sum += X[i][k] * Y[k][j];
            }
            R[i][j] = sum;
        }
    }
}

/* Compute the OEIS sequence A125418. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Base adjacency matrix is constant, build once. */
    static seq_t A[BASE][BASE];
    static bool built = false;
    if (!built) {
        built = true;
        for (size_t i = 0; i < BASE; ++i) {
            for (size_t j = 0; j < BASE; ++j) {
                seq_t diff = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);
                A[i][j] = (diff <= 6) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    seq_t res[BASE][BASE];
    seq_t baseM[BASE][BASE];
    seq_t tmp[BASE][BASE];

    /* Identity matrix for res */
    for (size_t i = 0; i < BASE; ++i) {
        for (size_t j = 0; j < BASE; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            baseM[i][j] = A[i][j];
        }
    }

    idx_t e = n;
    while (e) {
        if (e & (idx_t)1) {
            mat_mul(tmp, res, baseM);          /* tmp = res * baseM */
            for (size_t i = 0; i < BASE; ++i)
                for (size_t j = 0; j < BASE; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(tmp, baseM, baseM);              /* tmp = baseM * baseM */
        for (size_t i = 0; i < BASE; ++i)
            for (size_t j = 0; j < BASE; ++j)
                baseM[i][j] = tmp[i][j];
        e >>= 1;
    }

    /* trace of res gives total number of closed walks of length n */
    seq_t trace = (seq_t)0;
    for (size_t i = 0; i < BASE; ++i) {
        trace += res[i][i];
    }
    return trace;
}
