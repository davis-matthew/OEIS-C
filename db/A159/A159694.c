/*
 * A159694  a(n) = 2*a(n-1) + 2^(n-1), for n > 0, with a(0)=6.
 * https://oeis.org/A159694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (12 + n)*2^(n-1), a(0) = 6
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)6;
    }
    seq_t term = (seq_t)n + (seq_t)12;   /* (12 + n) */
    seq_t pow  = pow2(n - 1);            /* 2^(n-1) */
    return term * pow;                   /* (12 + n)*2^(n-1) */
}
