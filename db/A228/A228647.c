/*
 * A228647  a(n) = A001609(n^2) for n>=1, where g.f. of A001609 is
 *          x*(1+3*x^2)/(1-x-x^3).
 * https://oeis.org/A228647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001609(n^2), where A001609 is given by the recurrence
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=4 and for n ≥ 4, a(n)=a(n-1)+a(n-3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* required by the build system, the header is harmless */

typedef struct {          /* 3×3 matrix of seq_t */
    seq_t m[3][3];
} mat3;

/* Returns the identity matrix. */
static mat3 mat3_identity(void) {
    mat3 I = { {{1,0,0},
                {0,1,0},
                {0,0,1}} };
    return I;
}

/* Matrix multiplication: C = A * B */
static void mat3_mul(const mat3 *A, const mat3 *B, mat3 *C) {
    mat3 R;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += A->m[i][k] * B->m[k][j];
            }
            R.m[i][j] = sum;
        }
    }
    *C = R;
}

/* Computes a(m) = A001609(m) for m ≥ 0 (0‑based index).  */
static seq_t a001609_by_index(idx_t m) {
    if (m == 0) return 0;
    if (m == 1) return 1;
    if (m == 2) return 1;
    if (m == 3) return 4;

    /* Transition matrix for a(n) = a(n-1) + a(n-3) (n ≥ 4). */
    static const mat3 trans = { {{1,0,1},
                                 {1,0,0},
                                 {0,1,0}} };

    idx_t exp = (idx_t)(m - 3);        /* m ≥ 4, so exp fits in idx_t */
    mat3 result = mat3_identity();
    mat3 base   = trans;

    /* Fast exponentiation of the 3×3 matrix. */
    while (exp) {
        if (exp & 1ULL) {
            mat3 tmp;
            mat3_mul(&result, &base, &tmp);
            result = tmp;
        }
        mat3 tmp;
        mat3_mul(&base, &base, &tmp);
        base = tmp;
        exp >>= 1;
    }

    /* Multiply the resulting matrix by the initial vector [a3,a2,a1]ᵀ
       where a3=4, a2=1, a1=1. The first component gives a(m). */
    seq_t a_m = result.m[0][0] * 4 + result.m[0][1] * 1 + result.m[0][2] * 1;
    return a_m;
}

/* Main required function */
seq_t a(idx_t n) {
    /* Compute m = n² using seq_t to avoid overflow. */
    seq_t m = (seq_t)n * (seq_t)n;
    return a001609_by_index((idx_t)m);   /* idx_t(m) is safe because m fits in uint64 for the test range */
}
