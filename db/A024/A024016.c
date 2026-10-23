/*
 * A024016  a(n) = 2^n-n^6.
 * https://oeis.org/A024016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n^6 */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t n3   = n2   * nseq;
    seq_t n4   = n3   * nseq;
    seq_t n5   = n4   * nseq;
    seq_t n6   = n5   * nseq;

    /* compute 2^n by exponentiation by squaring */
    seq_t pow2 = 1;
    seq_t base = 2;
    idx_t exp  = n;                 /* use a copy for the loop */
    while (exp) {
        if (exp & 1) pow2 *= base;
        base *= base;
        exp >>= 1;
    }

    /* return the difference */
    return pow2 - n6;
}
