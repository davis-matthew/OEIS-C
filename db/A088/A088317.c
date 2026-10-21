/*
 * A088317  a(n) = 8*a(n-1) + a(n-2), starting with a(0) = 1 and a(1) = 4.
 * https://oeis.org/A088317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A088317
    a(0) = 1, a(1) = 4
    a(n) = 8*a(n-1) + a(n-2)    (n >= 2)
    The recurrence is homogeneous and linear; it can be solved with
    matrix exponentiation:
        [a(n)  ]   =  [[8,1],[1,0]]^(n-1)  * [a(1)]
        [a(n-1)]      [[1,0]]                      [a(0)]
    Therefore a(n) = M00*4 + M01*1 where M = [[8,1],[1,0]]^(n-1).
    This O(log n) algorithm works for any seq_t type (unsigned 128‑bit
    or big‑int). */

#include <stddef.h>  /* for size_t */

typedef struct { seq_t a00, a01, a10, a11; } mat2x2;

/* Multiply two 2×2 matrices over seq_t */
static mat2x2 mat_mul(const mat2x2 *x, const mat2x2 *y)
{
    mat2x2 r;
    r.a00 = x->a00*y->a00 + x->a01*y->a10;
    r.a01 = x->a00*y->a01 + x->a01*y->a11;
    r.a10 = x->a10*y->a00 + x->a11*y->a10;
    r.a11 = x->a10*y->a01 + x->a11*y->a11;
    return r;
}

/* Power a 2×2 matrix to exponent e (e >= 0) */
static mat2x2 mat_pow(mat2x2 base, idx_t e)
{
    mat2x2 result = {1, 0, 0, 1}; /* identity matrix */
    while (e) {
        if (e & 1) result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        e >>= 1;
    }
    return result;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    mat2x2 M = {8, 1, 1, 0};
    mat2x2 P = mat_pow(M, n - 1);

    /* a(n) = M00*4 + M01*1 */
    return P.a00 * (seq_t)4 + P.a01 * (seq_t)1;
}
