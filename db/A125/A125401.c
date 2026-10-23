/*
 * A125401  Number of base 13 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 13×13 adjacency matrix with
 *            A_ij = 1 if |i-j| ≤ 6, and 0 otherwise. Note that a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 (the empty circular number). */
    if (n == 0) return (seq_t)1;

    /* Constant size of the adjacency matrix (13 digits). */
    static const idx_t D = 13;

    /* B: current power of the adjacency matrix (initially A). */
    seq_t B[D][D];
    /* Initialize B to A. */
    for (idx_t i = 0; i < D; ++i) {
        for (idx_t j = 0; j < D; ++j) {
            idx_t diff = (i > j ? i - j : j - i);
            B[i][j] = (diff <= (idx_t)6) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* res: result matrix, initially the identity. */
    seq_t res[D][D];
    for (idx_t i = 0; i < D; ++i) {
        for (idx_t j = 0; j < D; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Binary exponentiation: compute A^n. */
    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            /* res = res * B */
            seq_t tmp[D][D];
            for (idx_t i = 0; i < D; ++i) {
                for (idx_t j = 0; j < D; ++j) {
                    seq_t sum = (seq_t)0;
                    for (idx_t k = 0; k < D; ++k) {
                        sum += res[i][k] * B[k][j];
                    }
                    tmp[i][j] = sum;
                }
            }
            /* copy back to res */
            for (idx_t i = 0; i < D; ++i) {
                for (idx_t j = 0; j < D; ++j) {
                    res[i][j] = tmp[i][j];
                }
            }
        }

        /* B = B * B */
        seq_t tmpB[D][D];
        for (idx_t i = 0; i < D; ++i) {
            for (idx_t j = 0; j < D; ++j) {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < D; ++k) {
                    sum += B[i][k] * B[k][j];
                }
                tmpB[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < D; ++i) {
            for (idx_t j = 0; j < D; ++j) {
                B[i][j] = tmpB[i][j];
            }
        }

        exp >>= 1;
    }

    /* Sum the diagonal of res: trace(A^n). */
    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < D; ++i) {
        trace += res[i][i];
    }
    return trace;
}
