/*
 * A125454  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1. A is the 19×19 adjacency
 *            matrix with A[i][j] = 1 iff |i−j| ≤ 8, otherwise 0. This counts
 *            n‑digit circular base‑19 numbers whose adjacent digits differ by
 *            at most 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* internal helper: matrix multiplication of 19×19 matrices */
static void mat_mul(const seq_t *A, const seq_t *B, seq_t *C)
{
    /* C = A * B   (all matrices 19×19, stored row‑major) */
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 19; ++k) {
                sum += A[i * 19 + k] * B[k * 19 + j];
            }
            C[i * 19 + j] = sum;
        }
    }
}

/* internal helper: copy a 19×19 matrix */
static void mat_copy(seq_t *dst, const seq_t *src)
{
    for (int i = 0; i < 361; ++i) dst[i] = src[i];
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {            /* empty number */
        return (seq_t)1;
    }

    /* build base adjacency matrix */
    seq_t base[361];
    for (int i = 0; i < 361; ++i) base[i] = 0;
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            int diff = (i > j) ? i - j : j - i;
            if (diff <= 8)
                base[i * 19 + j] = (seq_t)1;
        }
    }

    /* result = identity matrix */
    seq_t result[361], temp[361];
    for (int i = 0; i < 361; ++i) result[i] = 0;
    for (int i = 0; i < 19; ++i) result[i * 19 + i] = (seq_t)1;

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            mat_mul(result, base, temp);
            mat_copy(result, temp);
        }
        /* base = base * base */
        mat_mul(base, base, temp);
        mat_copy(base, temp);

        exp >>= 1;
    }

    /* trace of result */
    seq_t trace = 0;
    for (int i = 0; i < 19; ++i) {
        trace += result[i * 19 + i];
    }
    return trace;
}
