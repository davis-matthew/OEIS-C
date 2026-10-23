/*
 * A154383  Powers of 4 at even indices, two times powers of 4 at odd indices.
 * https://oeis.org/A154383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2n)=4^n, a(2n+1)=4^n / 2 (for n>0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 = 4^0 */
        return pow4(0);
    if (n == 1)            /* a(1) = 0, exception for n=1 */
        return (seq_t)0;

    if (n & 1U) {          /* odd index: n = 2k+1, k>0 */
        idx_t k = (n - 1) >> 1;   /* k = (n-1)/2 */
        seq_t v = pow4(k);        /* 4^k */
        return v / 2;             /* divide by 2 */
    } else {               /* even index: n = 2k */
        idx_t k = n >> 1;          /* k = n/2 */
        return pow4(k);            /* 4^k */
    }
}
