/*
 * A131118  a(4n) = -n^2, a(4n+1) = n^2, a(4n+2) = 1-n^2, a(4n+3) = n*(n+1).
 * https://oeis.org/A131118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4n) = -n^2, a(4n+1) = n^2, a(4n+2) = 1-n^2, a(4n+3) = n*(n+1)
 * Range:     i8 n <= 46, i16 n <= 726, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 2;           /* floor division by 4 */
    idx_t r = n & 3;            /* remainder modulo 4 */
    seq_t kv = (seq_t)k;        /* convert to seq_t */

    switch (r)
    {
        case 0:
            return -kv * kv;                 /* a(4k)   = -k^2 */
        case 1:
            return kv * kv;                  /* a(4k+1) =  k^2 */
        case 2:
            return (seq_t)1 - kv * kv;       /* a(4k+2) = 1 - k^2 */
        default:
            return kv * (kv + (seq_t)1);     /* a(4k+3) =  k(k+1) */
    }
}
