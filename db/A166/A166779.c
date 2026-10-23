/*
 * A166779  Number of nX2 1..6 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n-2)/(226800) * (2*n^8 + 106*n^7 + 2819*n^6 +
 *            41065*n^5 + 264893*n^4 + 73939*n^3 - 3091374*n^2 + 1937430*n -
 *            113400) (valid for n >= 1, with a(1)=a(2)=0)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t N = n;
    seq_t t = N - 1;
    seq_t u = N - 2;

    /* Horner's method for the polynomial */
    seq_t f = (seq_t)2;
    f = f * N + (seq_t)106;
    f = f * N + (seq_t)2819;
    f = f * N + (seq_t)41065;
    f = f * N + (seq_t)264893;
    f = f * N + (seq_t)73939;
    f = f * N - (seq_t)3091374;   /* subtraction instead of adding negative */
    f = f * N + (seq_t)1937430;
    f = f * N - (seq_t)113400;    /* subtraction instead of adding negative */

    /* multiply by (n-1)*(n-2) and divide by 226800 */
    f *= t;
    f *= u;
    return f / (seq_t)226800;
}
