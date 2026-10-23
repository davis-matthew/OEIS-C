/*
 * A125374  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A125374: Number of base‑11 circular n‑digit numbers with
   adjacent digits differing by at most 5.
   Formula: for n≥1, a(n) = trace(A^n), where A[i][j] = 1 if |i−j|≤5,
   0 otherwise.  a(0) = 1.  */

/* The types seq_t (the sequence values) and idx_t (the index) are
   supplied by the build system.  No other external symbols are used. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define DIGITS 11

/* Helper to multiply two 11×11 matrices in place:
   res = a × b.  All intermediate calculations use seq_t arithmetic. */
static void mat_mul(const seq_t a[DIGITS][DIGITS],
                    const seq_t b[DIGITS][DIGITS],
                    seq_t res[DIGITS][DIGITS]) {
    for (size_t i = 0; i < DIGITS; ++i) {
        for (size_t j = 0; j < DIGITS; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < DIGITS; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

/* The adjacency matrix A (constant).  It is built on first use. */
static seq_t base_matrix[DIGITS][DIGITS];
static bool initialized = false;

static void init_base(void) {
    if (initialized) return;
    for (size_t i = 0; i < DIGITS; ++i) {
        for (size_t j = 0; j < DIGITS; ++j) {
            size_t diff = (i > j) ? (i - j) : (j - i);
            base_matrix[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
        }
    }
    initialized = true;
}

/* Fast exponentiation of the base matrix.  Returns A^n in 'res'. */
static void power_matrix(idx_t n, seq_t res[DIGITS][DIGITS]) {
    /* result = Identity matrix */
    for (size_t i = 0; i < DIGITS; ++i) {
        for (size_t j = 0; j < DIGITS; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* power = base_matrix */
    seq_t power[DIGITS][DIGITS];
    for (size_t i = 0; i < DIGITS; ++i)
        for (size_t j = 0; j < DIGITS; ++j)
            power[i][j] = base_matrix[i][j];

    seq_t temp[DIGITS][DIGITS];

    while (n > (idx_t)0) {
        if (n & (idx_t)1) {
            mat_mul(res, power, temp);
            for (size_t i = 0; i < DIGITS; ++i)
                for (size_t j = 0; j < DIGITS; ++j)
                    res[i][j] = temp[i][j];
        }
        mat_mul(power, power, temp);
        for (size_t i = 0; i < DIGITS; ++i)
            for (size_t j = 0; j < DIGITS; ++j)
                power[i][j] = temp[i][j];
        n >>= (idx_t)1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;

    init_base();

    seq_t mat[DIGITS][DIGITS];
    power_matrix(n, mat);

    seq_t trace = (seq_t)0;
    for (size_t i = 0; i < DIGITS; ++i)
        trace += mat[i][i];
    return trace;
}
