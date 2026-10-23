/*
 * A174997  Integer part of the greatest eigenvalues of the matrix n X n whose
 *          elements are the Fibonacci numbers F(n) (A000045) such that n X n
 *          =
 *          ((F(0),F(1),...,F(n-1)),(F(n),F(n+1),...,F(2n-1)),...,(F(n(n-1)),F(n(n-1)+1),...,F(n^2-1))),
 *          for n=1,2,...
 * https://oeis.org/A174997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{n-1} F((n+1)*i) Using matrix powers: a(n) is
 *            the (0,1)-entry of sum_{i=0}^{n-1} X^i where X =
 *            [[1,1],[1,0]]^(n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t a00, a01, a10, a11; } matrix;

static inline matrix mat_zero() { return (matrix){0,0,0,0}; }
static inline matrix mat_identity() { return (matrix){1,0,0,1}; }

static inline matrix mat_add(const matrix *m1, const matrix *m2)
{
    return (matrix){
        m1->a00 + m2->a00,
        m1->a01 + m2->a01,
        m1->a10 + m2->a10,
        m1->a11 + m2->a11
    };
}

static inline matrix mat_mul(const matrix *m1, const matrix *m2)
{
    return (matrix){
        m1->a00 * m2->a00 + m1->a01 * m2->a10,
        m1->a00 * m2->a01 + m1->a01 * m2->a11,
        m1->a10 * m2->a00 + m1->a11 * m2->a10,
        m1->a10 * m2->a01 + m1->a11 * m2->a11
    };
}

/* Fast exponentiation of a 2x2 matrix */
static inline matrix mat_pow(matrix base, idx_t exp)
{
    matrix result = mat_identity();
    while (exp) {
        if (exp & 1) result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Recursive computation of geometric sum of matrix powers
   For a given matrix X and positive integer m,
   returns in *S the sum I + X + X^2 + ... + X^{m-1}
   and in *P the matrix X^m. */
static inline void geom_sum(const matrix *X, idx_t m, matrix *S, matrix *P)
{
    if (m == 0) {
        *S = mat_zero();
        *P = mat_identity();
        return;
    }
    if (m == 1) {
        *S = mat_identity();
        *P = *X;
        return;
    }
    if ((m & 1) == 0) {                /* m is even */
        matrix S_half, P_half;
        geom_sum(X, m >> 1, &S_half, &P_half);
        matrix temp = mat_mul(&P_half, &S_half);
        *S = mat_add(&S_half, &temp);
        *P = mat_mul(&P_half, &P_half);
    } else {                            /* m is odd */
        matrix S_prev, P_prev;
        geom_sum(X, m - 1, &S_prev, &P_prev);
        *S = mat_add(&S_prev, &P_prev);
        *P = mat_mul(&P_prev, X);
    }
}

seq_t a(idx_t n)
{
    /* Q = [[1,1],[1,0]] */
    matrix Q = {1, 1, 1, 0};
    /* X = Q^(n+1) */
    matrix X   = mat_pow(Q, n + 1);
    /* S = sum_{i=0}^{n-1} X^i */
    matrix S, P;
    geom_sum(&X, n, &S, &P);
    /* a(n) is the (0,1) entry of S */
    return S.a01;
}
