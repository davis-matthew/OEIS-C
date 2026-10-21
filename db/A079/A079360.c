/*
 * A079360  Sequence of sums of alternating increasing powers of 2.
 * https://oeis.org/A079360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 6*2^n - 5, a(2n-1) = 5*(2^n - 1)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if ((n & (idx_t)1) == 0) {           /* n even: a(2k) */
        idx_t k = n >> 1;                /* k = n/2 */
        seq_t p = pow2(k);               /* p = 2^k */
        return (seq_t)6 * p - (seq_t)5;
    } else {                              /* n odd: a(2k+1) */
        idx_t k = (n >> 1) + 1;           /* k = (n+1)/2 */
        seq_t p = pow2(k);               /* p = 2^k */
        return (seq_t)5 * (p - (seq_t)1);
    }
}
