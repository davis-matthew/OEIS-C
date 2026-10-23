/*
 * A194721  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting nine doublets into the
 *          initially empty word.
 * https://oeis.org/A194721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 16t + 119t^2 + 544t^3 + 1700t^4 + 3808t^5 + 6188t^6 +
 *            7072t^7 + 4862t^8 where t = n-1 (for n>0), a(0) = 0. This is a
 *            polynomial of degree 8, evaluated by Horner’s rule
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 89, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* t = n-1, n>0 guarantees no underflow */
    seq_t t = (seq_t)(n - (idx_t)1);

    /* coefficients of the polynomial from highest to lowest degree
       (excluding the constant term which is added after the loop). */
    static const seq_t coeffs[8] = {
        (seq_t)4862,  /* t^8 coefficient */
        (seq_t)7072,  /* t^7 */
        (seq_t)6188,  /* t^6 */
        (seq_t)3808,  /* t^5 */
        (seq_t)1700,  /* t^4 */
        (seq_t)544,   /* t^3 */
        (seq_t)119,   /* t^2 */
        (seq_t)16     /* t^1 */
    };

    /* Horner’s rule */
    seq_t val = coeffs[0];
    for (int i = 1; i < 8; ++i)
        val = val * t + coeffs[i];
    val = val * t + (seq_t)1;   /* add constant term */

    return val;
}
