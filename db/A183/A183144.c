/*
 * A183144  [1/s]+[2/s]+...+[n/s], where s=(3+sqrt(3))/2, []=floor.
 * https://oeis.org/A183144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 - ∑_{k=1..n} floor(k*sqrt(3)/3)
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k <= n; ++k) {
        /* compute floor(k*sqrt(3)/3) without floating point
           floor(k*sqrt(3)) = floor(sqrt(3*k*k)) */
        seq_t t   = (seq_t)3 * k * k;          /* 3*k^2 */
        seq_t sq  = floor_sqrt(t);             /* floor(sqrt(3*k^2)) */
        seq_t f   = sq / 3;                    /* floor( sqrt(3*k^2) / 3 ) */
        sum += f;
    }
    seq_t nSeq = (seq_t)n;
    seq_t half = nSeq * (nSeq - 1) / 2;
    return half - sum;
}
