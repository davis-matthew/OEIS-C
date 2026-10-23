/*
 * A159415  Number of n X n arrays of squares of integers summing to 25.
 * https://oeis.org/A159415
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^25 in (1 + x + x^4 + x^9 + x^16 +
 *            x^25)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>                       /* for memset */

#define DEG 25

/* Multiply two polynomials A and B (degree ≤ DEG),
   store the truncated product (degree ≤ DEG) in C. */
static void mul_poly(const seq_t *A, const seq_t *B, seq_t *C)
{
    idx_t i, j;
    /* zero C */
    for (i = 0; i <= DEG; ++i)
        C[i] = 0;

    for (i = 0; i <= DEG; ++i) {
        if (A[i] == 0)
            continue;
        for (j = 0; j <= DEG && i + j <= DEG; ++j) {
            C[i + j] += A[i] * B[j];
        }
    }
}

/* Copy an array of length DEG+1 */
static void copy_poly(seq_t *dst, const seq_t *src)
{
    idx_t i;
    for (i = 0; i <= DEG; ++i)
        dst[i] = src[i];
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* undefined for n < 2 */

    idx_t i;
    seq_t base[DEG + 1];
    seq_t result[DEG + 1];
    seq_t temp[DEG + 1];
    idx_t exp = n * n;                       /* number of cells in the array */

    /* initialise arrays */
    for (i = 0; i <= DEG; ++i) {
        base[i]   = 0;
        result[i] = 0;
    }
    result[0] = 1;                           /* start with polynomial 1 */

    /* base polynomial: 1 + x + x^4 + x^9 + x^16 + x^25 */
    base[0] = 1;
    base[1] = 1;
    base[4] = 1;
    base[9] = 1;
    base[16] = 1;
    base[25] = 1;

    /* exponentiation by repeated squaring */
    while (exp > 0) {
        if (exp & 1) {
            mul_poly(result, base, temp);
            copy_poly(result, temp);
        }
        mul_poly(base, base, temp);
        copy_poly(base, temp);
        exp >>= 1;
    }

    return result[25];
}
