/*
 * A131711  Period 12: repeat 0, 1, 2, 5, 2, 9, 0, 9, 8, 5, 8, 1.
 * https://oeis.org/A131711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131711: last digit of Pell numbers
   Recurrence: P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2)
   a(n)=P(n) mod 10
   The sequence is periodic with period 12:
   0,1,2,5,2,9,0,9,8,5,8,1
   This implementation uses fast‑matrix exponentiation to
   compute the result in O(log n) time for any n≥0. */

#include <stdlib.h>   /* for size_t, realloc */

static inline seq_t mod10(seq_t x) {
    return x % ((seq_t)10);
}

typedef struct {
    seq_t a00, a01;
    seq_t a10, a11;
} mat2_t;

/* matrix multiplication modulo 10 */
static inline mat2_t mat_mul(mat2_t A, mat2_t B) {
    mat2_t C;
    C.a00 = mod10(A.a00 * B.a00 + A.a01 * B.a10);
    C.a01 = mod10(A.a00 * B.a01 + A.a01 * B.a11);
    C.a10 = mod10(A.a10 * B.a00 + A.a11 * B.a10);
    C.a11 = mod10(A.a10 * B.a01 + A.a11 * B.a11);
    return C;
}

/* fast exponentiation of a 2×2 matrix modulo 10 */
static mat2_t mat_pow(mat2_t base, idx_t exp) {
    mat2_t result = {1, 0, 0, 1};          /* identity matrix */
    while (exp) {
        if (exp & 1) result = mat_mul(result, base);
        base   = mat_mul(base, base);
        exp >>= 1;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* transition matrix for Pell numbers */
    mat2_t M = {2, 1,
                1, 0};

    /* compute M^(n-1) */
    mat2_t P = mat_pow(M, n - 1);

    /* P(n) = first component of (M^(n-1) * [P1, P0]^T)
       where P1=1, P0=0, so simply P(n)=P.a00 */
    return mod10(P.a00);
}
