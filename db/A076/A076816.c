/*
 * A076816  Squares modulo triangular numbers: n^2 minus the greatest
 *          triangular number smaller than or equal to n^2.
 * https://oeis.org/A076816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n^2 - floor((sqrt(8*n^2+1)-1)/2) *
 *            (floor((sqrt(8*n^2+1)-1)/2)+1) / 2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for arbitrary-precision seq_t values. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = (seq_t)3 * x + 1;          /* high > sqrt(x) for positive x (sqrt(8*n^2+1) < 3*n) */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;                           /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;                         /* n^2                */
    seq_t val  = (seq_t)8 * n_sq + 1;             /* 8*n^2 + 1          */
    seq_t m    = isqrt_seq(val);                  /* floor(sqrt(8*n^2+1)) */
    seq_t k    = (m - 1) / 2;                     /* floor((m-1)/2)     */
    seq_t tri  = k * (k + 1) / 2;                 /* triangular number  */
    return n_sq - tri;                            /* a(n)                */
}
