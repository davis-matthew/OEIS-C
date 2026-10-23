/*
 * A137445  a(n) = 2a(n-1)-2a(n-2), with a(0)=3 and a(1)=2.
 * https://oeis.org/A137445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2a(n-1)-2a(n-2), a(0)=3, a(1)=2
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t m[2][2];
} mat2;

static mat2 mul(mat2 a, mat2 b) {
    mat2 r;
    r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0];
    r.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1];
    r.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0];
    r.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1];
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;

    idx_t exp = n - 1;
    mat2 M, R;
    M.m[0][0] = (seq_t)2;
    M.m[0][1] = (seq_t)-2;
    M.m[1][0] = (seq_t)1;
    M.m[1][1] = (seq_t)0;
    R.m[0][0] = (seq_t)1;
    R.m[0][1] = (seq_t)0;
    R.m[1][0] = (seq_t)0;
    R.m[1][1] = (seq_t)1;

    while (exp > 0) {
        if (exp & 1) {
            R = mul(R, M);
        }
        M = mul(M, M);
        exp >>= 1;
    }

    seq_t a1 = (seq_t)2;
    seq_t a0 = (seq_t)3;
    return R.m[0][0] * a1 + R.m[0][1] * a0;
}
