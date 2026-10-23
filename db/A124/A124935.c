/*
 * A124935  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) for n >= 1, one special case a(0)=1. A is the
 *            18×18 matrix with A[i][j] = 1 if |i-j| ≤ 2 (and 0 otherwise)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* matrix multiplication (18×18) */
static void mat_mul(const seq_t a[18][18], const seq_t b[18][18], seq_t res[18][18])
{
    for (int i = 0; i < 18; ++i) {
        for (int j = 0; j < 18; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 18; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

/* initialize adjacency matrix A (once) */
static void init_matrix(seq_t A[18][18])
{
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 18; ++j)
            A[i][j] = 0;

    for (int i = 0; i < 18; ++i)
        for (int d = -2; d <= 2; ++d) {
            int j = i + d;
            if (j >= 0 && j < 18)
                A[i][j] = 1;
        }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* offset 0 */

    /* static storage for the base matrix */
    static seq_t A[18][18];
    static int init = 0;
    if (!init) {
        init_matrix(A);
        init = 1;
    }

    /* identity matrix for the result */
    seq_t res[18][18], base[18][18], tmp[18][18];
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 18; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* copy A to base */
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 18; ++j)
            base[i][j] = A[i][j];

    idx_t e = n;
    while (e) {
        if (e & 1u) {                     /* res *= base */
            mat_mul(res, base, tmp);
            for (int i = 0; i < 18; ++i)
                for (int j = 0; j < 18; ++j)
                    res[i][j] = tmp[i][j];
        }
        /* base *= base */
        mat_mul(base, base, tmp);
        for (int i = 0; i < 18; ++i)
            for (int j = 0; j < 18; ++j)
                base[i][j] = tmp[i][j];
        e >>= 1;
    }

    /* trace of A^n */
    seq_t total = 0;
    for (int i = 0; i < 18; ++i)
        total += res[i][i];
    return total;
}
