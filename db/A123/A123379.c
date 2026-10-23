/*
 * A123379  Values x of the solutions (x,y) of the Diophantine equation
 *          5*(X-Y)^4 - 4*X*Y = 0 with X >= Y.
 * https://oeis.org/A123379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 340*a(n-1) - 5798*a(n-2) + 340*a(n-3) - a(n-4) with
 *            a(0)=0, a(1)=20, a(2)=5832, a(3)=1866940
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct { seq_t m[4][4]; } Mat;

static const seq_t init_vals[4] = { 0, 20, 5832, 1866940 };

static void mat_mul(const Mat *A, const Mat *B, Mat *C)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += A->m[i][k] * B->m[k][j];
            }
            C->m[i][j] = sum;
        }
    }
}

static void mat_pow(Mat base, idx_t exp, Mat *out)
{
    Mat result;
    /* identity matrix */
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = (i == j) ? 1 : 0;

    while (exp > 0) {
        if (exp & 1ULL) {
            Mat tmp;
            mat_mul(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            Mat tmp;
            mat_mul(&base, &base, &tmp);
            base = tmp;
        }
    }
    *out = result;
}

seq_t a(idx_t n)
{
    if (n < 4) return init_vals[n];

    /* Transition matrix M for the recurrence */
    Mat M;
    M.m[0][0] = 340;   M.m[0][1] = -5798;  M.m[0][2] = 340;   M.m[0][3] = -1;
    M.m[1][0] = 1;     M.m[1][1] = 0;      M.m[1][2] = 0;      M.m[1][3] = 0;
    M.m[2][0] = 0;     M.m[2][1] = 1;      M.m[2][2] = 0;      M.m[2][3] = 0;
    M.m[3][0] = 0;     M.m[3][1] = 0;      M.m[3][2] = 1;      M.m[3][3] = 0;

    /* Compute M^(n-3) */
    Mat Mp;
    mat_pow(M, n - 3, &Mp);

    /* Initial state vector S(3) = [a3, a2, a1, a0] */
    seq_t S[4] = { init_vals[3], init_vals[2], init_vals[1], init_vals[0] };

    /* a(n) is the first component of Mp * S */
    seq_t result = 0;
    for (int i = 0; i < 4; ++i) {
        result += Mp.m[0][i] * S[i];
    }
    return result;
}
