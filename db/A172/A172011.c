/*
 * A172011  a(n) = 12*A002605(n).
 * https://oeis.org/A172011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2), a(0)=0, a(1)=12
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 2×2 matrix for the linear recurrence a(n)=2a(n‑1)+2a(n‑2)   */
typedef struct {
    seq_t a00, a01, a10, a11;
} mat_t;

/* matrix multiplication (2×2) */
static mat_t mat_mul(mat_t m1, mat_t m2)
{
    mat_t r;
    r.a00 = m1.a00 * m2.a00 + m1.a01 * m2.a10;
    r.a01 = m1.a00 * m2.a01 + m1.a01 * m2.a11;
    r.a10 = m1.a10 * m2.a00 + m1.a11 * m2.a10;
    r.a11 = m1.a10 * m2.a01 + m1.a11 * m2.a11;
    return r;
}

/* returns the n‑th term of A172011 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 12;
    }

    /* base matrix [[2,2],[1,0]] represents the recurrence */
    mat_t base = {2, 2, 1, 0};
    mat_t result = {1, 0, 0, 1};   /* identity matrix */

    idx_t exp = n - 1;
    while (exp > 0) {
        if (exp & 1) {
            result = mat_mul(result, base);
        }
        base = mat_mul(base, base);
        exp >>= 1;
    }

    /* a(n) = result.a00 * a(1) + result.a01 * a(0) = result.a00 * 12   */
    seq_t value = result.a00 * (seq_t)12;
    return value;
}
