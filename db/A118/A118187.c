/*
 * A118187  Antidiagonal sums of triangle A118185: a(n) = Sum_{k=0..[n/2]}
 *          4^(k*(n-2*k)) for n>=0.
 * https://oeis.org/A118187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 4^(k*(n-2k))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n >> 1;                     /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k) {
        idx_t e = k * (n - 2 * k);           /* k*(n-2k) */
        /* 4^e = 2^(2e) */
        seq_t term = pow2(2 * e);
        sum += term;
    }
    return sum;
}
