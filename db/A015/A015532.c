/*
 * A015532  a(n) = 4*a(n-1) + 7*a(n-2).
 * https://oeis.org/A015532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 7*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by matrix exponentiation in O(log n) time.  */
#include <stddef.h>

/* Helper structure for a 2x2 matrix of seq_t values.  */
typedef struct {
    seq_t a00, a01, a10, a11;
} mat2;

static mat2 mat_mul(mat2 x, mat2 y)
{
    mat2 r;
    r.a00 = x.a00 * y.a00 + x.a01 * y.a10;
    r.a01 = x.a00 * y.a01 + x.a01 * y.a11;
    r.a10 = x.a10 * y.a00 + x.a11 * y.a10;
    r.a11 = x.a10 * y.a01 + x.a11 * y.a11;
    return r;
}

static mat2 mat_pow(mat2 base, idx_t exp)
{
    mat2 result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* identity */
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result = mat_mul(result, base);
        }
        base = mat_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    mat2 T = { (seq_t)4, (seq_t)7,
               (seq_t)1, (seq_t)0 };
    mat2 P = mat_pow(T, n-1);

    /* a(n) = P[0][0]*a(1) + P[0][1]*a(0) = P[0][0] */
    return P.a00;
}
