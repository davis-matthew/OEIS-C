/*
 * A123335  a(n) = -2*a(n-1) + a(n-2) for n>1, a(0)=1, a(1)=-1.
 * https://oeis.org/A123335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-2), a(0)=1, a(1)=-1
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 50, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two 2×2 matrices (row‑major order).   res = a * b */
static inline void mat_mul(seq_t *res, const seq_t *a, const seq_t *b)
{
    seq_t t0 = a[0] * b[0] + a[1] * b[2];
    seq_t t1 = a[0] * b[1] + a[1] * b[3];
    seq_t t2 = a[2] * b[0] + a[3] * b[2];
    seq_t t3 = a[2] * b[1] + a[3] * b[3];
    res[0] = t0;  res[1] = t1;  res[2] = t2;  res[3] = t3;
}

/* Fast exponentiation of 2×2 matrix.  res = base^exp */
static inline void mat_pow(seq_t *res, const seq_t *base, idx_t exp)
{
    seq_t R[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };   /* identity */
    seq_t B[4] = { base[0], base[1], base[2], base[3] };

    while (exp)
    {
        if (exp & (idx_t)1)
            mat_mul(R, R, B);
        mat_mul(B, B, B);
        exp >>= (idx_t)1;
    }
    res[0] = R[0]; res[1] = R[1]; res[2] = R[2]; res[3] = R[3];
}

/* OEIS A123335 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)-1;

    /* Transition matrix for the recurrence */
    seq_t T[4] = { (seq_t)-2, (seq_t)1, (seq_t)1, (seq_t)0 };

    seq_t P[4];
    mat_pow(P, T, n - (idx_t)1);

    /* Compose with vector [a(1), a(0)] = [-1, 1] */
    seq_t ans = P[0] * (seq_t)-1 + P[1] * (seq_t)1;
    return ans;
}
