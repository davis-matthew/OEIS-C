/*
 * A233148  Number of n-edge-colorings of the cubical graph.
 * https://oeis.org/A233148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n^9 -21*n^8 +203*n^7 -1191*n^6 +4701*n^5
 *            -13031*n^4 +25524*n^3 -34192*n^2 +28400*n -11072)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 42, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t sn = (seq_t)n;          /* n          */
    seq_t sn1 = sn - 1;          /* n - 1      */
    seq_t sn2 = sn - 2;          /* n - 2      */

    /* powers of n */
    seq_t pow2 = sn * sn;       /* n^2   */
    seq_t pow3 = pow2 * sn;     /* n^3   */
    seq_t pow4 = pow3 * sn;     /* n^4   */
    seq_t pow5 = pow4 * sn;     /* n^5   */
    seq_t pow6 = pow5 * sn;     /* n^6   */
    seq_t pow7 = pow6 * sn;     /* n^7   */
    seq_t pow8 = pow7 * sn;     /* n^8   */
    seq_t pow9 = pow8 * sn;     /* n^9   */

    /* positive part of the polynomial */
    seq_t pos = pow9;
    pos += (seq_t)203 * pow7;
    pos += (seq_t)4701 * pow5;
    pos += (seq_t)25524 * pow3;
    pos += (seq_t)28400 * sn;

    /* negative part of the polynomial */
    seq_t neg = (seq_t)21 * pow8;
    neg += (seq_t)1191 * pow6;
    neg += (seq_t)13031 * pow4;
    neg += (seq_t)34192 * pow2;
    neg += (seq_t)11072;

    /* polynomial value P(n) = pos - neg */
    seq_t p = pos - neg; /* pos >= neg for all n >= 3 */

    /* a(n) = n*(n-1)*(n-2)*P(n) */
    seq_t pre = sn * sn1 * sn2;
    return pre * p;
}
