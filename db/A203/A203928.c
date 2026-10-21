/*
 * A203928  Number of (n+1) X 3 0..5 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) + 70*a(n-2) (n ≥ 3) a(1) = 4356, a(2) = 106596
 *            (Descending order recurrence, 2‑term linear)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for NULL, if needed */

struct Matrix {
    seq_t a00, a01, a10, a11;
};

/* 2×2 matrix multiplication */
static inline struct Matrix mat_mul(const struct Matrix *x, const struct Matrix *y)
{
    struct Matrix r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

/* binary exponentiation of a 2×2 matrix */
static inline struct Matrix mat_pow(struct Matrix base, idx_t exp)
{
    struct Matrix result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* identity */
    while (exp > 0) {
        if (exp & (idx_t)1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A203928 */
seq_t a(idx_t n)
{
    /* Handle the offset 1 explicitly */
    if (n == 1) return (seq_t)4356;
    if (n == 2) return (seq_t)106596;

    /* Transition matrix for the recurrence */
    const struct Matrix M = { (seq_t)21, (seq_t)70, (seq_t)1, (seq_t)0 };

    /* Use matrix exponentiation: [a_n, a_{n-1}]^T = M^{n-2} * [a_2, a_1]^T */
    struct Matrix P = mat_pow(M, n - 2);

    const seq_t a2 = (seq_t)106596;
    const seq_t a1 = (seq_t)4356;

    /* a_n = P.a00*a2 + P.a01*a1 */
    return P.a00 * a2 + P.a01 * a1;
}
