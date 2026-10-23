/*
 * A124956  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| ≤ 2, else 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base 28, circular n‑digit numbers with adjacent digits differing by at most 2. */

#define N 28

/* Perform matrix multiplication: res = a * b */
static void mat_mul(const seq_t a[N][N], const seq_t b[N][N], seq_t res[N][N]) {
    int i, j, k;
    /* initialise result to zero */
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            res[i][j] = 0;
        }
    }
    /* multiply */
    for (i = 0; i < N; ++i) {
        for (k = 0; k < N; ++k) {
            if (a[i][k] == 0) continue;
            for (j = 0; j < N; ++j) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Base adjacency matrix A, built once */
    static seq_t A[N][N];
    static int A_init = 0;
    if (!A_init) {
        int i, j;
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                int d = (i > j) ? (i - j) : (j - i);
                A[i][j] = (d <= 2) ? (seq_t)1 : (seq_t)0;
            }
        }
        A_init = 1;
    }

    /* matrices for exponentiation */
    seq_t result[N][N];
    seq_t powA[N][N];
    seq_t temp[N][N];
    int i, j;

    /* initialise result = I, powA = A */
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            powA[i][j]   = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            mat_mul(result, powA, temp);
            for (i = 0; i < N; ++i)
                for (j = 0; j < N; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(powA, powA, temp);
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j)
                powA[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* trace of result gives a(n) */
    seq_t sum = 0;
    for (i = 0; i < N; ++i) sum += result[i][i];
    return sum;
}
