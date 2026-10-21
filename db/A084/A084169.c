/*
 * A084169  A Pell Jacobsthal product.
 * https://oeis.org/A084169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001045(n) * A000129(n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements Pell and Jacobsthal numbers via matrix exponentiation. */

#include <stdlib.h>
#include <stdint.h>

typedef struct { seq_t a[2][2]; } Mat;

/* Multiply two 2x2 matrices of type seq_t */
static Mat mat_mul(const Mat *x, const Mat *y)
{
    Mat z;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 2; ++k) {
                sum += x->a[i][k] * y->a[k][j];
            }
            z.a[i][j] = sum;
        }
    }
    return z;
}

/* Fast exponentiation of a 2x2 matrix (base^exp). */
static Mat mat_pow(Mat base, idx_t exp)
{
    Mat result = { { {1, 0}, {0, 1} } };   /* Identity matrix */

    while (exp) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Compute Pell number P(n) :  P(0)=0, P(1)=1, P(n)=2P(n-1)+P(n-2) */
static seq_t pell(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    Mat m = { { {2, 1}, {1, 0} } };   /* Transition matrix for Pell */
    Mat r = mat_pow(m, n - 1);
    return r.a[0][0];
}

/* Compute Jacobsthal number J(n) : J(0)=0, J(1)=1, J(n)=J(n-1)+2J(n-2) */
static seq_t jacobsthal(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    Mat m = { { {1, 1}, {2, 0} } };   /* Transition matrix for Jacobsthal */
    Mat r = mat_pow(m, n - 1);
    return r.a[0][0];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t p = pell(n);
    seq_t j = jacobsthal(n);
    return p * j;
}
