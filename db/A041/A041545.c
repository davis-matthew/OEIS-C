/*
 * A041545  Denominators of continued fraction convergents to sqrt(290).
 * https://oeis.org/A041545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 34*a(n-1) + a(n-2); a(0)=1, a(1)=34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct { seq_t m00, m01, m10, m11; } mat2;

static mat2 mul(mat2 A, mat2 B) {
    mat2 C;
    C.m00 = A.m00 * B.m00 + A.m01 * B.m10;
    C.m01 = A.m00 * B.m01 + A.m01 * B.m11;
    C.m10 = A.m10 * B.m00 + A.m11 * B.m10;
    C.m11 = A.m10 * B.m01 + A.m11 * B.m11;
    return C;
}

static mat2 mpow(mat2 base, idx_t exp) {
    mat2 res = {1, 0, 0, 1};   /* identity matrix */
    while (exp) {
        if (exp & 1U) res = mul(res, base);
        base = mul(base, base);
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n) {
    static const mat2 M = {34, 1, 1, 0};
    mat2 P = mpow(M, n);
    /* For n = 0, mpow returns the identity whose m00 == 1 */
    return P.m00;
}
