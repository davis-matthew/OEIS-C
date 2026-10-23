/*
 * A125359  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i - j| ≤ 4 else 0,
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define SZ 22

/* Multiply two SZ×SZ matrices over seq_t.  Result in out. */
static void mat_mult(const seq_t a[SZ][SZ], const seq_t b[SZ][SZ], seq_t out[SZ][SZ]) {
    for (idx_t i = 0; i < SZ; ++i) {
        for (idx_t j = 0; j < SZ; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < SZ; ++k)
                sum += a[i][k] * b[k][j];
            out[i][j] = sum;
        }
    }
}

/* Initialize a matrix to the identity. */
static void mat_identity(seq_t out[SZ][SZ]) {
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            out[i][j] = (i == j) ? 1 : 0;
}

/* Count circular base‑22 n‑digit numbers whose adjacent digits differ by ≤4. */
seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Build adjacency matrix once. */
    static seq_t A[SZ][SZ];
    static int init = 0;
    if (!init) {
        init = 1;
        for (idx_t i = 0; i < SZ; ++i)
            for (idx_t j = 0; j < SZ; ++j)
                A[i][j] = ((i > j ? i - j : j - i) <= 4) ? 1 : 0;
    }

    seq_t R[SZ][SZ], M[SZ][SZ], T[SZ][SZ];
    mat_identity(R);
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            M[i][j] = A[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mult(R, M, T);
            for (idx_t i = 0; i < SZ; ++i)
                for (idx_t j = 0; j < SZ; ++j)
                    R[i][j] = T[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mult(M, M, T);
            for (idx_t i = 0; i < SZ; ++i)
                for (idx_t j = 0; j < SZ; ++j)
                    M[i][j] = T[i][j];
        }
    }

    seq_t result = 0;
    for (idx_t i = 0; i < SZ; ++i)
        result += R[i][i];
    return result;
}
