/*
 * A107271  Let M = the 3 X 3 matrix [1 1 1; 3 1 0; 2 0 0]. Perform M^n * [1 0
 *          0] getting (1, 3, 2; 6, 6, 2; 14, 24, 12; 50, 66, 28; ...) which
 *          we string together to form the sequence.
 * https://oeis.org/A107271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (M^ceil(n/3) * [1,0,0])[(n-1) mod 3] where M =
 *            {{1,1,1},{3,1,0},{2,0,0}}
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* needed only for interface consistency */

/* multiply 3×3 matrices (no modulus) */
static void mat_mul(const seq_t A[3][3], const seq_t B[3][3], seq_t C[3][3])
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* multiply 3×3 matrix with 3×1 vector */
static void mat_vec_mul(const seq_t M[3][3], const seq_t v[3], seq_t r[3])
{
    for (int i = 0; i < 3; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 3; ++j) {
            sum += M[i][j] * v[j];
        }
        r[i] = sum;
    }
}

seq_t a(idx_t n)
{
    /* trivial handling for indices below the offset */
    if (n == 0) return 0;

    /* block number k = ceil(n/3) = (n+2)/3 for integer division */
    idx_t k = (n + 2) / 3;

    /* position inside the 3‑term subset ((n-1) mod 3) */
    unsigned rem = (unsigned)((n - 1) % 3);

    /* initial values */
    static const seq_t M[3][3] = {
        {1, 1, 1},
        {3, 1, 0},
        {2, 0, 0}
    };
    seq_t r[3] = {1, 0, 0};

    /* current power of M */
    seq_t cur[3][3];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            cur[i][j] = M[i][j];

    /* fast exponentiation applying to the vector */
    while (k > 0) {
        if (k & 1) {
            seq_t tmp[3];
            mat_vec_mul(cur, r, tmp);
            r[0] = tmp[0]; r[1] = tmp[1]; r[2] = tmp[2];
        }
        /* square the matrix */
        seq_t tmpM[3][3];
        mat_mul(cur, cur, tmpM);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                cur[i][j] = tmpM[i][j];
        k >>= 1;
    }

    return r[rem];
}
