/*
 * A042625  Denominators of continued fraction convergents to sqrt(842).
 * https://oeis.org/A042625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 58*a(n-1) + a(n-2), a(0)=1, a(1)=58
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

typedef struct { seq_t a00, a01, a10, a11; } mat_t;

/* Multiply two 2x2 matrices over seq_t */
static mat_t mat_mul(const mat_t *x, const mat_t *y)
{
    mat_t r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

/* Raise matrix 'base' to power 'exp' (exp >= 0) */
static mat_t mat_pow(mat_t base, idx_t exp)
{
    mat_t result = { .a00 = 1, .a01 = 0, .a10 = 0, .a11 = 1 };  /* identity */
    while (exp > 0)
    {
        if (exp & 1) result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)58;

    /* transition matrix M = [[58,1],[1,0]] */
    mat_t M = { .a00 = (seq_t)58, .a01 = (seq_t)1,
                .a10 = (seq_t)1,  .a11 = (seq_t)0 };

    /* compute M^(n-1) */
    mat_t P = mat_pow(M, n - 1);

    /* base vector v(1) = [58, 1]^T */
    seq_t a1 = 58, a0 = 1;

    /* a(n) = first component of P * v(1) */
    seq_t an = P.a00 * a1 + P.a01 * a0;
    return an;
}
