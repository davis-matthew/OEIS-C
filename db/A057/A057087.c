/*
 * A057087  Scaled Chebyshev U-polynomials evaluated at i. Generalized
 *          Fibonacci sequence.
 * https://oeis.org/A057087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(a(n-1) + a(n-2)); a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)4;
    }

    /* Fast matrix exponentiation for the recurrence
     *   a(n) = 4*a(n-1) + 4*a(n-2)
     * which corresponds to the matrix
     *
     *  |4 4|
     *  |1 0|
     *
     * Let M be this matrix.  Then
     *   [a(n)   ] = M^(n-1) * [a(1)]
     *   [a(n-1)]           [a(0)]
     *
     * Thus a(n) = (M^(n-1))[0][0]*a(1) + (M^(n-1))[0][1]*a(0)
     */

    idx_t power = n - (idx_t)1;           /* exponent for M */

    /* 2x2 matrix structure */
    struct Mat {
        seq_t a, b, c, d;                 /* rows: [a b] [c d] */
    } result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 },   /* identity */
           base   = { (seq_t)4, (seq_t)4, (seq_t)1, (seq_t)0 }; /* M */

    /* Exponentiation by squaring */
    while (power > (idx_t)0) {
        if (power & (idx_t)1) {
            /* result = result * base */
            struct Mat tmp;
            tmp.a = result.a * base.a + result.b * base.c;
            tmp.b = result.a * base.b + result.b * base.d;
            tmp.c = result.c * base.a + result.d * base.c;
            tmp.d = result.c * base.b + result.d * base.d;
            result = tmp;
        }
        /* base = base * base */
        struct Mat tmp2;
        tmp2.a = base.a * base.a + base.b * base.c;
        tmp2.b = base.a * base.b + base.b * base.d;
        tmp2.c = base.c * base.a + base.d * base.c;
        tmp2.d = base.c * base.b + base.d * base.d;
        base = tmp2;

        power >>= (idx_t)1;
    }

    /* Compute a(n) from the resulting matrix */
    const seq_t a0 = (seq_t)1;
    const seq_t a1 = (seq_t)4;
    return result.a * a1 + result.b * a0;
}
