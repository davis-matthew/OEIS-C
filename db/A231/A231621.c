/*
 * A231621  a(n) = A000930(n*(n+1)/2), where A000930 is Narayana's cows
 *          sequence.
 * https://oeis.org/A231621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000930(n*(n+1)/2), where A000930 is Narayana's cows
 *            sequence: f(0)=f(1)=f(2)=1, f(m)=f(m-1)+f(m-3) for m≥3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 3x3 matrix over seq_t */
typedef struct { seq_t m[3][3]; } mat3;

/* Companion matrix for the recurrence f(m)=f(m-1)+f(m-3) */
static const mat3 NARAYANA_MATRIX = {
    { { (seq_t)1, (seq_t)0, (seq_t)1 },
      { (seq_t)1, (seq_t)0, (seq_t)0 },
      { (seq_t)0, (seq_t)1, (seq_t)0 } }
};

/* Identity matrix */
static const mat3 IDENTITY_MATRIX = {
    { { (seq_t)1, (seq_t)0, (seq_t)0 },
      { (seq_t)0, (seq_t)1, (seq_t)0 },
      { (seq_t)0, (seq_t)0, (seq_t)1 } }
};

/* Matrix multiplication over seq_t */
static mat3 mat_mul(const mat3 *a, const mat3 *b)
{
    mat3 r;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += a->m[i][k] * b->m[k][j];
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

/* Binary exponentiation of the companion matrix */
static mat3 mat_pow(idx_t exp)
{
    mat3 result = IDENTITY_MATRIX;
    mat3 base   = NARAYANA_MATRIX;
    while (exp) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Return f(k) where f is Narayana's cows sequence */
static seq_t narayana_of(idx_t k)
{
    /* The third row of NARAYANA_MATRIX^k multiplied by the vector
       [f(2), f(1), f(0)] = [1,1,1] gives f(k).  Since the vector is
       all ones, f(k) equals the sum of the entries of the third row. */
    mat3 mp = mat_pow(k);
    return mp.m[2][0] + mp.m[2][1] + mp.m[2][2];
}

seq_t a(idx_t n)
{
    /* Compute the triangular number T_n = n*(n+1)/2  using seq_t arithmetic. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    /* The triangular number fits in idx_t for all inputs common to the build. */
    return narayana_of((idx_t)tri);
}
