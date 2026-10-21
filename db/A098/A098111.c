/*
 * A098111  Inverse binomial transform of A098149.
 * https://oeis.org/A098111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1 a(1) = 0 a(n) = 5 * (a(n-1) - a(n-2)) for n >= 2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t a, b, c, d; /* represents [[a,b],[c,d]] */
} Matrix;

/* matrix multiplication: res = x * y */
static Matrix mul(const Matrix *x, const Matrix *y) {
    Matrix res;
    res.a = x->a * y->a + x->b * y->c;
    res.b = x->a * y->b + x->b * y->d;
    res.c = x->c * y->a + x->d * y->c;
    res.d = x->c * y->b + x->d * y->d;
    return res;
}

/* fast exponentiation of a 2x2 matrix: base^exp */
static Matrix mat_pow(const Matrix *base, idx_t exp) {
    Matrix result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* identity */
    Matrix cur   = *base;
    while (exp > 0) {
        if (exp & (idx_t)1) result = mul(&result, &cur);
        cur = mul(&cur, &cur);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)0;

    /* companion matrix for the recurrence a(n) = 5*(a(n-1)-a(n-2)) */
    Matrix M = { (seq_t)5, (seq_t)-5, (seq_t)1, (seq_t)0 };

    /* compute M^(n-1) */
    Matrix Mp = mat_pow(&M, n - 1);

    /* a(n) = first component of (M^(n-1) * [a(1), a(0)])^T
       Since [a(1), a(0)] = [0, 1], the answer is Mp.b */
    return Mp.b;
}
