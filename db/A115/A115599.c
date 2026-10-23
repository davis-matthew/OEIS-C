/*
 * A115599  Consider all Pythagorean triples (X,X+1,Z) ordered by increasing
 *          Z; sequence gives Z-X values.
 * https://oeis.org/A115599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115599: a(1)=2, a(2)=9, a(3)=50;
   recurrence: a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3)   (n ≥ 4)   */

/* 3×3 matrix of seq_t values */
typedef struct {
    seq_t m[3][3];
} Mat3;

/* multiply two 3×3 matrices */
static inline Mat3 mat_mul(const Mat3 *x, const Mat3 *y)
{
    Mat3 r;
    for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (unsigned k = 0; k < 3; ++k)
                sum += x->m[i][k] * y->m[k][j];
            r.m[i][j] = sum;
        }
    }
    return r;
}

/* fast exponentiation of a 3×3 matrix */
static inline Mat3 mat_pow(Mat3 base, idx_t exp)
{
    Mat3 res = { { {1,0,0}, {0,1,0}, {0,0,1} } };   /* identity matrix */
    while (exp > 0) {
        if (exp & 1)
            res = mat_mul(&res, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n=0; return 0 as a harmless value */
        return 0;
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)9;
    if (n == 3)
        return (seq_t)50;

    /* companion matrix for the recurrence */
    Mat3 M = { { {7, (seq_t)(-7), 1},
                 {1, 0, 0},
                 {0, 1, 0} } };

    /* V_3 = [a(3), a(2), a(1)]^T = [50, 9, 2] */
    seq_t v0[3] = { (seq_t)50, (seq_t)9, (seq_t)2 };

    /* Compute M^(n-3) * V_3 */
    Mat3 p = mat_pow(M, n - 3);

    /* first component of the resulting vector = a(n) */
    seq_t result = p.m[0][0] * v0[0] + p.m[0][1] * v0[1] + p.m[0][2] * v0[2];
    return result;
}
