/*
 * A140044  Sequence generated from the 5/5Z addition table considered as a
 *          matrix.
 * https://oeis.org/A140044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (1,2)-entry of the n-th power of the 5×5 matrix * [0
 *            1 2 3 4] * [1 2 3 4 0] * [2 3 4 0 1] * [3 4 0 1 2] * [4 0 1 2 3]
 *            * i.e. a(n) = term (1,2) of Xⁿ, where X is that addition table
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mul_matrix(const seq_t a[25], const seq_t b[25], seq_t c[25]) {
    seq_t temp[25];
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            seq_t sum = 0;
            int idx_i = i * 5;
            for (int k = 0; k < 5; ++k) {
                sum += a[idx_i + k] * b[k * 5 + j];
            }
            temp[idx_i + j] = sum;
        }
    }
    for (int i = 0; i < 25; ++i) {
        c[i] = temp[i];
    }
}

seq_t a(idx_t n) {
    /* base matrix X */
    seq_t X[25] = {
        0, 1, 2, 3, 4,
        1, 2, 3, 4, 0,
        2, 3, 4, 0, 1,
        3, 4, 0, 1, 2,
        4, 0, 1, 2, 3
    };

    /* result matrix R initialized to identity */
    seq_t R[25] = {0};
    for (int i = 0; i < 5; ++i) {
        R[i * 5 + i] = 1;
    }

    /* working matrix A = X */
    seq_t A[25];
    for (int i = 0; i < 25; ++i) {
        A[i] = X[i];
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            mul_matrix(R, A, R);   /* R = R * A */
        mul_matrix(A, A, A);       /* A = A * A */
        exp >>= 1;
    }

    return R[0 * 5 + 1]; /* (1,2)-entry (0-based indices) */
}
