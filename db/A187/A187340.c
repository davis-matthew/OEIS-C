/*
 * A187340  Hankel transform of A014301(n+1).
 * https://oeis.org/A187340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1-2n)*cos(Pi*n/3)+(2n+3)*sin(Pi*n/3)/sqrt(3) Rewritten
 *            using periodic cos/sin values: r = n mod 6 r==0: a(n) = 1 - 2n
 *            r==1: a(n) = 2 r==2: a(n) = 2n + 1 r==3: a(n) = 2n - 1 r==4:
 *            a(n) = -2 r==5: a(n) = -(2n + 1)
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    seq_t two_n = (seq_t)n * 2;   /* 2*n as seq_t */

    switch (r) {
        case 0: return (seq_t)1 - two_n;               /* 1 - 2n */
        case 1: return (seq_t)2;                       /* constant 2 */
        case 2: return two_n + (seq_t)1;              /* 2n + 1 */
        case 3: return two_n - (seq_t)1;              /* 2n - 1 */
        case 4: return -(seq_t)2;                     /* -2 */
        case 5: return -(two_n + (seq_t)1);           /* -(2n + 1) */
        default: return (seq_t)0; /* unreachable, keeps compiler happy */
    }
}
