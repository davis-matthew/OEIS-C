/*
 * A125322  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1, where A[i][j] = 1 if
 *            |i-j| ≤ 3, else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

/* 12×12 matrix type */
typedef seq_t mat[12][12];

/* matrix multiplication: C = A * B */
static void mat_mul(const mat A, const mat B, mat C)
{
    int i, j, k;
    for (i = 0; i < 12; ++i) {
        for (j = 0; j < 12; ++j) {
            C[i][j] = 0;
            for (k = 0; k < 12; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/* copy matrix src into dst */
static void mat_copy(mat dst, const mat src)
{
    int i, j;
    for (i = 0; i < 12; ++i) {
        for (j = 0; j < 12; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}

/* matrix power: result = pow(A, exp)   (exponentiation by squaring) */
static void mat_pow(const mat base, idx_t exp, mat result)
{
    mat res, cur, temp;
    int i, j;

    /* initialize res = I (identity matrix) */
    for (i = 0; i < 12; ++i) {
        for (j = 0; j < 12; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }

    /* copy base into cur */
    mat_copy(cur, base);

    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, cur, temp);
            mat_copy(res, temp);
        }
        mat_mul(cur, cur, temp);
        mat_copy(cur, temp);
        exp >>= 1;
    }

    /* final result */
    mat_copy(result, res);
}

/* compute a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;   /* by definition a(0) = 1 */
    }

    /* adjacency matrix A */
    static const mat BASE = {
        {1,1,1,1,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,1,1,1,1,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,1,1,1,1}
    };

    mat power;
    mat_pow(BASE, n, power);

    /* trace of A^n */
    seq_t ans = 0;
    int i;
    for (i = 0; i < 12; ++i) {
        ans += power[i][i];
    }
    return ans;
}
