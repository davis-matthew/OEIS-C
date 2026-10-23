/*
 * A135647  Expansion of 1 / ((25*x^10+49*x-1) * (x^10+2*x-1)^3 *
 *          (x^10+x-1)^6).
 * https://oeis.org/A135647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = coefficient of x^n in the expansion of
 *            1/((25*x^10+49*x-1)*(x^10+2*x-1)^3*(x^10+x-1)^6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static void poly_mul(const seq_t *x, idx_t lx,
                     const seq_t *y, idx_t ly,
                     seq_t *z)
{
    /* z[0 .. lx+ly] will be computed; z is assumed zeroed for that range */
    idx_t i, j;
    for (i = 0; i <= lx + ly; ++i) z[i] = 0;
    for (i = 0; i <= lx; ++i) {
        for (j = 0; j <= ly; ++j) {
            z[i + j] += x[i] * y[j];
        }
    }
}

seq_t a(idx_t n)
{
    const idx_t MAX_DEG = 100;

    /* Coefficients of the denominator polynomial D(x) = (A)*(B)^3*(C)^6   */
    seq_t d[MAX_DEG + 1];
    idx_t i, j, m, k;

    /* Initialize all coefficient arrays to zero */
    for (i = 0; i <= MAX_DEG; ++i) d[i] = 0;

    /* Base polynomials A, B, C */
    seq_t A[11] = {0}, B[11] = {0}, C[11] = {0};
    A[0] = -1; A[1] = 49;   A[10] = 25;
    B[0] = -1; B[1] =  2;   B[10] =  1;
    C[0] = -1; C[1] =  1;   C[10] =  1;

    /* Temporary polynomial arrays */
    seq_t T1[21], T2[31], T3[21], T4[41], T5[61], T6[41];

    /* Compute B^2, then B^3 */
    poly_mul(B, 10, B, 10, T1);          /* T1 = B^2, degree 20 */
    poly_mul(T1, 20, B, 10, T2);         /* T2 = B^3, degree 30 */

    /* Compute C^2, then C^4, then C^6 */
    poly_mul(C, 10, C, 10, T3);          /* T3 = C^2, degree 20 */
    poly_mul(T3, 20, T3, 20, T4);        /* T4 = C^4, degree 40 */
    poly_mul(T4, 40, T3, 20, T5);        /* T5 = C^6, degree 60 */

    /* Compute A * B^3 */
    poly_mul(A, 10, T2, 30, T6);         /* T6 = A * B^3, degree 40 */

    /* Compute D = (A * B^3) * C^6 */
    poly_mul(T6, 40, T5, 60, d);         /* d[0..100] holds D(x) */

    /* Series inversion: 1/D(x) = f(x) with f[0] = 1 */
    if (n == 0) return (seq_t)1;

    seq_t *a_arr = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
    if (!a_arr) return 0; /* memory allocation failed */
    a_arr[0] = (seq_t)1;

    for (k = 1; k <= n; ++k) {
        seq_t sum = 0;
        idx_t lim = (k < MAX_DEG) ? k : MAX_DEG;
        for (i = 1; i <= lim; ++i) {
            sum += d[i] * a_arr[k - i];
        }
        a_arr[k] = -sum;  /* since d[0] == 1 */
    }

    seq_t result = a_arr[n];
    free(a_arr);
    return result;
}
