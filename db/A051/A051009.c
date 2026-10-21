/*
 * A051009  Reduced denominators of Newton's iteration for sqrt(2).
 * https://oeis.org/A051009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Pell(2^(n-1)) (Pell numbers: P(0)=0, P(1)=1,
 *            P(k)=2P(k-1)+P(k-2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used in idx_t (provided by the build) */

typedef struct {
    seq_t a00, a01;   /* first row      */
    seq_t a10, a11;   /* second row     */
} mat2;

/* multiply two 2×2 matrices over seq_t */
static inline mat2 mat_mul(const mat2 *x, const mat2 *y)
{
    mat2 r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

/* Compute the n-th term of A051009 (n≥1) */
seq_t a(idx_t n)
{
    /* a(1)=1: base matrix M = [[2,1],[1,0]] represents the recurrence
       for Pell numbers: [P(k+1), P(k)]^T = M * [P(k), P(k-1)]^T */
    mat2 M = { (seq_t)2, (seq_t)1,
              (seq_t)1, (seq_t)0 };

    /* compute M^(2^(n-1)) by repeated squaring:
       start with exponent 1 (M), each squaring doubles the exponent.
       After n-1 squarings we have exponent 2^(n-1). */
    mat2 R = M;
    for (idx_t i = 1; i < n; ++i)
        R = mat_mul(&R, &R);

    /* [P(k+1), P(k)]^T = R * [P(1)=1, P(0)=0]^T
       Thus the desired term a(n) = P(2^(n-1)) is R.a10. */
    return R.a10;
}
