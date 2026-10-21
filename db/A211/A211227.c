/*
 * A211227  Row sums of A211226.
 * https://oeis.org/A211227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>=1, if n is odd, a(n)=2^((n+1)/2); if n is even,
 *            a(n)=(n/2+2)*2^((n/2)-1)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    if (n & 1U) { /* odd index */
        idx_t k = (n + 1) >> 1U;      /* k = (n+1)/2  */
        return pow_int((seq_t)2, k);
    } else { /* even index */
        idx_t k = n >> 1U;                 /* k = n/2      */
        seq_t pow = pow_int((seq_t)2, k - 1); /* 2^(k-1)      */
        return (seq_t)(k + 2U) * pow;
    }
}
