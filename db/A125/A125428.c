/*
 * A125428  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Tr(A^n) for n>0, where A[i][j]=1 if |i-j|≤7 and 0
 *            otherwise. a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence a(n) for base 16 circular n-digit numbers. */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Dimension of the adjacency matrix. */
    const int D = 16;

    /* Matrix type. */
    typedef struct {
        seq_t m[D][D];
    } Mat;

    /* Construct the base adjacency matrix. */
    Mat base;
    for (int i = 0; i < D; ++i) {
        for (int j = 0; j < D; ++j) {
            int diff = i > j ? i - j : j - i;
            base.m[i][j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Identity matrix. */
    Mat result;
    for (int i = 0; i < D; ++i) {
        for (int j = 0; j < D; ++j) {
            result.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Fast exponentiation to compute base^n. */
    idx_t exp = n;
    Mat tmp;
    while (exp) {
        if (exp & 1) {
            /* tmp = result * base */
            for (int i = 0; i < D; ++i) {
                for (int j = 0; j < D; ++j) {
                    seq_t acc = 0;
                    for (int k = 0; k < D; ++k) {
                        acc += result.m[i][k] * base.m[k][j];
                    }
                    tmp.m[i][j] = acc;
                }
            }
            result = tmp;
        }
        /* base = base * base */
        for (int i = 0; i < D; ++i) {
            for (int j = 0; j < D; ++j) {
                seq_t acc = 0;
                for (int k = 0; k < D; ++k) {
                    acc += base.m[i][k] * base.m[k][j];
                }
                tmp.m[i][j] = acc;
            }
        }
        base = tmp;
        exp >>= 1;
    }

    /* Sum diagonal entries to get the trace. */
    seq_t ans = 0;
    for (int i = 0; i < D; ++i) {
        ans += result.m[i][i];
    }
    return ans;
}
