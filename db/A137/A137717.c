/*
 * A137717  Hankel transform of A106191.
 * https://oeis.org/A137717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - 4*a(n-2), a(0) = 1, a(1) = -4
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) via fast matrix exponentiation */

typedef struct {
    seq_t a00, a01, a10, a11;
} mat_t;

static inline mat_t mat_mul(mat_t x, mat_t y)
{
    mat_t r;
    r.a00 = x.a00 * y.a00 + x.a01 * y.a10;
    r.a01 = x.a00 * y.a01 + x.a01 * y.a11;
    r.a10 = x.a10 * y.a00 + x.a11 * y.a10;
    r.a11 = x.a10 * y.a01 + x.a11 * y.a11;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;

    idx_t p = n - 1;
    mat_t res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* identity */
    mat_t base = { (seq_t)-2, (seq_t)-4, (seq_t)1, (seq_t)0 };

    while (p) {
        if (p & (idx_t)1)
            res = mat_mul(res, base);
        base = mat_mul(base, base);
        p >>= 1;
    }

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)-4;
    return res.a00 * a1 + res.a01 * a0;
}
