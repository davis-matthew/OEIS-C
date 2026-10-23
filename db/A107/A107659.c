/*
 * A107659  a(n) = Sum_{k=0..n} 2^max(k, n-k).
 * https://oeis.org/A107659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} 2^max(k, n-k)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(2m) = 2^m (2^(m+2) - 3),   a(2m+1) = 2^m (2^(m+3) - 4) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* 2^m */
    seq_t pow_m = pow2(m);

    if ((n & 1) == 0) {
        /* Even case: a(2m) = 2^m (2^(m+2) - 3) */
        seq_t pow_m_plus2 = pow2(m + 2);
        return pow_m * (pow_m_plus2 - (seq_t)3);
    } else {
        /* Odd case: a(2m+1) = 2^m (2^(m+3) - 4) */
        seq_t pow_m_plus3 = pow2(m + 3);
        return pow_m * (pow_m_plus3 - (seq_t)4);
    }
}
