/*
 * A125489  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of base‑32 circular n‑digit numbers whose
 *            adjacent digits differ by at most 9. a(0) = 1 For n ≥ 1, a(n) =
 *            trace( M^n ) where M[i][j] = 1 if |i – j| ≤ 9, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   absdiff_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/absdiff_int.h"   /* for absdiff_int */

#define DIGITS 32
#define DIFF_LIMIT 9

static seq_t M[DIGITS][DIGITS];
static seq_t I[DIGITS][DIGITS];
static bool _initialized = false;

/* initialise the adjacency matrix M and identity I */
static void init_matrix(void) {
    for (idx_t i = 0; i < DIGITS; ++i) {
        for (idx_t j = 0; j < DIGITS; ++j) {
            M[i][j] = absdiff_int((int)i, (int)j) <= DIFF_LIMIT ? (seq_t)1 : (seq_t)0;
        }
    }
    for (idx_t i = 0; i < DIGITS; ++i) {
        for (idx_t j = 0; j < DIGITS; ++j) {
            I[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
    _initialized = true;
}

/* multiply two 32×32 matrices: out = A × B  */
static void mat_mult_out(seq_t out[DIGITS][DIGITS],
                         const seq_t A[DIGITS][DIGITS],
                         const seq_t B[DIGITS][DIGITS]) {
    seq_t tmp[DIGITS][DIGITS];
    for (idx_t i = 0; i < DIGITS; ++i) {
        for (idx_t j = 0; j < DIGITS; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < DIGITS; ++k) {
                sum += A[i][k] * B[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (idx_t i = 0; i < DIGITS; ++i) {
        for (idx_t j = 0; j < DIGITS; ++j) {
            out[i][j] = tmp[i][j];
        }
    }
}

/* copy a matrix: dst = src  */
static void mat_copy(seq_t dst[DIGITS][DIGITS],
                     const seq_t src[DIGITS][DIGITS]) {
    for (idx_t i = 0; i < DIGITS; ++i) {
        for (idx_t j = 0; j < DIGITS; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}

/* public function to compute the sequence term */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (!_initialized) init_matrix();

    seq_t result[DIGITS][DIGITS];
    seq_t base[DIGITS][DIGITS];
    seq_t tmp[DIGITS][DIGITS];

    mat_copy(result, I);          /* identity matrix */
    mat_copy(base, M);            /* adjacency matrix */

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mult_out(tmp, result, base);
            mat_copy(result, tmp);
        }
        mat_mult_out(tmp, base, base);
        mat_copy(base, tmp);
        exp >>= 1;
    }

    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < DIGITS; ++i) {
        trace += result[i][i];
    }
    return trace;
}
