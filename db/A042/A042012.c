/*
 * A042012  Numerators of continued fraction convergents to sqrt(530).
 * https://oeis.org/A042012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 46*a(n-1) + a(n-2), a(0)=23, a(1)=1059
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t a00, a01, a10, a11;
} mat2;

static mat2 mat_mult(const mat2 *m1, const mat2 *m2)
{
    mat2 r;
    r.a00 = m1->a00 * m2->a00 + m1->a01 * m2->a10;
    r.a01 = m1->a00 * m2->a01 + m1->a01 * m2->a11;
    r.a10 = m1->a10 * m2->a00 + m1->a11 * m2->a10;
    r.a11 = m1->a10 * m2->a01 + m1->a11 * m2->a11;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    if (n == 1) return (seq_t)1059;

    mat2 base = { (seq_t)46, (seq_t)1, (seq_t)1, (seq_t)0 };
    mat2 res  = { (seq_t)1,  (seq_t)0,  (seq_t)0,  (seq_t)1 };
    idx_t exp = n - 1;

    while (exp)
    {
        if (exp & 1U) res = mat_mult(&res, &base);
        base = mat_mult(&base, &base);
        exp >>= 1;
    }

    /* multiply the resulting matrix by the vector [a(1), a(0)]^T */
    return res.a00 * (seq_t)1059 + res.a01 * (seq_t)23;
}
