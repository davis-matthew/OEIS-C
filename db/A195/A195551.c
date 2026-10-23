/*
 * A195551  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 3/2.
 * https://oeis.org/A195551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   b(n) = max((m^2 - n^2)/g , (2*m*n)/g) where (m , n) are the
 *            (n‑1)‑th convergents of r = (3+√13)/2. The convergents satisfy
 *            m0 = 3 , n0 = 1 m1 = 10, n1 = 3 m_{k+1} = 3*m_k + m_{k-1}
 *            n_{k+1} = 3*n_k + n_{k-1} The k‑th convergent corresponds to the
 *            n‑th member of A195551 for k = n-1. The numerator i...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t a00, a01, a10, a11;
} matrix2x2;

/* multiply two 2×2 matrices over seq_t */
static matrix2x2 mul_matrix(const matrix2x2 *x, const matrix2x2 *y)
{
    matrix2x2 r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

/* exponentiate matrix M to the power e (unsigned) */
static matrix2x2 pow_matrix(matrix2x2 base, idx_t e)
{
    matrix2x2 result = { .a00 = 1, .a01 = 0, .a10 = 0, .a11 = 1 };  /* identity */
    while (e > 0) {
        if (e & 1) result = mul_matrix(&result, &base);
        base = mul_matrix(&base, &base);
        e >>= 1;
    }
    return result;
}

/* sequence A195551 */
seq_t a(idx_t n)
{
    /* special case n == 1 */
    if (n == 1) return 4;   /* (m,n) = (3,1) */

    /* base convergents */
    seq_t m_prev = 10;   /* m1 */
    seq_t m_prev_prev = 3; /* m0 */
    seq_t n_prev = 3;   /* n1 */
    seq_t n_prev_prev = 1; /* n0 */

    if (n == 2) {
        /* use m1,n1 already stored */
    } else {
        /* compute (n-2) powers of the recurrence matrix */
        matrix2x2 M = { .a00 = 3, .a01 = 1, .a10 = 1, .a11 = 0 };
        idx_t e = n - 2;
        matrix2x2 P = pow_matrix(M, e);

        /* apply P to the initial vector [m1, m0] and [n1, n0] */
        seq_t m = P.a00 * m_prev + P.a01 * m_prev_prev;
        seq_t n = P.a00 * n_prev + P.a01 * n_prev_prev;

        /* update local variables for later computation */
        m_prev = m;
        n_prev = n;
    }

    /* compute legs of the Pythagorean triple */
    seq_t a_leg = m_prev * m_prev - n_prev * n_prev;   /* m^2 - n^2 */
    seq_t b_leg = 2 * m_prev * n_prev;                 /* 2*m*n */

    /* reduce to primitive triple */
    seq_t g = gcd(a_leg, b_leg);
    a_leg = a_leg / g;
    b_leg = b_leg / g;

    /* numerator is the larger leg */
    return (a_leg > b_leg) ? a_leg : b_leg;
}
