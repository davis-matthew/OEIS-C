/*
 * A042636  Numerators of continued fraction convergents to sqrt(848).
 * https://oeis.org/A042636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(848).  
   Computed by matrix exponentiation on the 6‑term period of the continued fraction. */

#include <stdint.h>

typedef struct {
    seq_t m[2][2];
} mat_t;

/* Matrix multiplication: C = A * B */
static mat_t mat_mul(const mat_t *A, const mat_t *B)
{
    mat_t C;
    C.m[0][0] = A->m[0][0] * B->m[0][0] + A->m[0][1] * B->m[1][0];
    C.m[0][1] = A->m[0][0] * B->m[0][1] + A->m[0][1] * B->m[1][1];
    C.m[1][0] = A->m[1][0] * B->m[0][0] + A->m[1][1] * B->m[1][0];
    C.m[1][1] = A->m[1][0] * B->m[0][1] + A->m[1][1] * B->m[1][1];
    return C;
}

/* Matrix exponentiation by squaring: result = base^exp */
static mat_t mat_pow(mat_t base, idx_t exp)
{
    mat_t result = { .m = { {1,0},{0,1} } }; /* identity matrix */
    while (exp != 0) {
        if (exp & 1) {
            result = mat_mul(&result, &base);
        }
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Coefficients of the 6‑term period of the continued fraction of sqrt(848) */
    static const seq_t coeff[6] = { (seq_t)8, (seq_t)3, (seq_t)3, (seq_t)3, (seq_t)8, (seq_t)58 };

    /* base of the convergents: a0 = 29 */
    const seq_t a0 = (seq_t)29;

    /* Compute q = n / 6 and r = n % 6 */
    idx_t q = n / 6;
    idx_t r = n % 6;

    /* Compute the period matrix P = M6 * M5 * ... * M1 */
    mat_t period = { .m = { {1,0},{0,1} } }; /* identity */
    for (int i = 0; i < 6; ++i) {
        mat_t M = { .m = { { coeff[i], 1 }, { 1, 0 } } };
        period = mat_mul(&M, &period);          /* left multiplication */
    }

    /* Period matrix raised to the qth power */
    mat_t Pq = mat_pow(period, q);

    /* Compute the product of the first r matrices R = Mr * ... * M1 */
    mat_t R = { .m = { {1,0},{0,1} } }; /* identity */
    for (int i = 0; i < (int)r; ++i) {
        mat_t M = { .m = { { coeff[i], 1 }, { 1, 0 } } };
        R = mat_mul(&M, &R);          /* left multiplication */
    }

    /* Total transformation matrix for n steps */
    mat_t total = mat_mul(&R, &Pq);

    /* The numerator is the first component of total * [a0, 1]^T */
    seq_t result = total.m[0][0] * a0 + total.m[0][1] * (seq_t)1;
    return result;
}
