/*
 * A165938  a(n) = A002203(n^2) for n>=1.
 * https://oeis.org/A165938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1+sqrt(2))^(n^2) + (1-sqrt(2))^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the recurrence X_k = 2*X_{k-1} + X_{k-2}
   with X_0 = 2, X_1 = 2.  a(n) = X_{n^2}.  We compute X_{k} by
   matrix exponentiation of the companion matrix [[2,1],[1,0]]. */

#include <stdint.h>  /* for NULL (if needed) */

typedef struct {
    seq_t a, b, c, d;   /* 2x2 matrix: [ [a,b], [c,d] ] */
} mat2;

/* Matrix multiplication over seq_t */
static inline mat2 mat_mul(const mat2 *A, const mat2 *B) {
    mat2 C;
    C.a = A->a * B->a + A->b * B->c;
    C.b = A->a * B->b + A->b * B->d;
    C.c = A->c * B->a + A->d * B->c;
    C.d = A->c * B->b + A->d * B->d;
    return C;
}

/* Fast exponentiation of a 2x2 matrix */
static inline mat2 mat_pow(mat2 base, idx_t exp) {
    mat2 result = { .a = 1, .b = 0, .c = 0, .d = 1 };  /* Identity */
    while (exp) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute X_{n^2}.  For n>=1, k = n^2, exponent e = k-1. */
    idx_t k = (idx_t)n * n;        /* k = n^2  (fits in idx_t for n<=10^4) */
    idx_t e = k - 1;               /* exponent for the matrix power */

    mat2 M = { .a = 2, .b = 1, .c = 1, .d = 0 };   /* companion matrix */

    mat2 P = mat_pow(M, e);      /* M^{k-1} */

    /* Initial vector [X_1, X_0] = [2, 2] */
    seq_t two = 2;
    seq_t result = P.a * two + P.b * two;  /* first component of P * [2,2]^T */

    return result;
}
