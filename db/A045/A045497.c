/*
 * A045497  Packing density for n balls in complex projective plane using
 *          Fubini-Study metric (denominators).
 * https://oeis.org/A045497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A045497:  a(n) is the cumulative sum of the coefficients of
 *  x*(1 + x + 2*x^2 -3*x^3 + 24*x^4 + 39*x^6 + 225*x^7 - 288*x^8) /
 *  (1 - x).  For n ≥ 9 the sum equals 1.  */
/*  Reference:  http://oeis.org/A045497  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* if n ≥ 9, the sequence stabilises at 1 */
    if (n >= 9)
        return (seq_t)1;

    /* coefficients c_k of the numerator (k=1..9)  */
    static const int coeff[10] = {
        0,   /* unused  */
        1,   /* c1  */
        1,   /* c2  */
        2,   /* c3  */
        -3,  /* c4  */
        24,  /* c5  */
        0,   /* c6  */
        39,  /* c7  */
        225, /* c8  */
        -288 /* c9 */
    };

    /* compute the cumulative sum up to n (n ≤ 8) */
    int64_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += coeff[k];

    return (seq_t)sum;
}
