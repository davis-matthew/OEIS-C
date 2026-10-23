/*
 * A101355  a(n) = binomial(2*binomial(2*n,n-1),binomial(2*n,n-1)-1).
 * https://oeis.org/A101355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*binomial(2*n,n-1),binomial(2*n,n-1)-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   binom_big, binom_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_big.h"
#include "../utils/binom_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* sequence is defined for n >= 1 */
    seq_t m1 = (seq_t)2 * (seq_t)n;
    seq_t k1 = (seq_t)(n - 1);
    seq_t M   = binom_big(m1, k1);
    seq_t m2  = M + M;           /* 2*M */
    seq_t k2  = M - (seq_t)1;    /* M-1 */
    return binom_big(m2, k2);
}
