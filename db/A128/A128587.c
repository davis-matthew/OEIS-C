/*
 * A128587  Row sums of A128586.
 * https://oeis.org/A128587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) + a(n-3), with a(1)=a(2)=a(3)=1
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t m[3][3]; } mat3;

/* Multiply two 3×3 matrices of type seq_t */
static mat3 mat_mul(const mat3 *A, const mat3 *B) {
    mat3 C;
    for (idx_t i = 0; i < 3; ++i) {
        for (idx_t j = 0; j < 3; ++j) {
            seq_t s = 0;
            for (idx_t k = 0; k < 3; ++k)
                s = s + A->m[i][k] * B->m[k][j];
            C.m[i][j] = s;
        }
    }
    return C;
}

/* Raise matrix base to exponent exp (non‑negative) */
static mat3 mat_pow(mat3 base, idx_t exp) {
    mat3 result;
    /* identity matrix */
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            result.m[i][j] = (i == j) ? 1 : 0;

    while (exp > 0) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 3) return 1;

    /* Transition matrix for the recurrence a(n) = -2*a(n-1)+a(n-3) */
    mat3 M = { .m = { { -2, 0, 1 },
                      {  1, 0, 0 },
                      {  0, 1, 0 } } };

    idx_t k = n - 3;                     /* need M^k */
    mat3 R = mat_pow(M, k);

    /* Multiply R by vector [1, 1, 1]⁺ – sum of first row gives a(n) */
    return R.m[0][0] + R.m[0][1] + R.m[0][2];
}
