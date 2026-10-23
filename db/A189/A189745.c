/*
 * A189745  a(n) = 5*a(n-1) + a(n-2); with a(1)=5, a(2)=1.
 * https://oeis.org/A189745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2), a(1)=5, a(2)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t a00, a01, a10, a11;
} mat2;

// multiply two 2x2 matrices over seq_t
static void mat_mul(const mat2 *A, const mat2 *B, mat2 *C) {
    C->a00 = A->a00 * B->a00 + A->a01 * B->a10;
    C->a01 = A->a00 * B->a01 + A->a01 * B->a11;
    C->a10 = A->a10 * B->a00 + A->a11 * B->a10;
    C->a11 = A->a10 * B->a01 + A->a11 * B->a11;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)1;

    idx_t exp = n - 2;
    mat2 base = { (seq_t)5, (seq_t)1, (seq_t)1, (seq_t)0 };
    mat2 result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; // identity

    while (exp > 0) {
        if (exp & 1) {
            mat2 tmp;
            mat_mul(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            mat2 tmp;
            mat_mul(&base, &base, &tmp);
            base = tmp;
        }
    }

    /* Multiply the resulting matrix by the vector [a2, a1] = [1, 5] */
    return result.a00 * (seq_t)1 + result.a01 * (seq_t)5;
}
