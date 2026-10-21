/*
 * A060482  New record highs reached in A060030.
 * https://oeis.org/A060482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, a(2) = 2; for even n ≥ 4: a(n) = 2^(n/2+1) – 3; for
 *            odd n ≥ 3: a(n) = 3·2^((n-1)/2) – 3
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if ((n & 1) == 0) {                    /* even n ≥ 4  */
        idx_t k = n / 2;                   /* k ≥ 2        */
        seq_t val = pow2(k + 1);           /* 2^(k+1)      */
        val -= (seq_t)3;
        return val;
    } else {                               /* odd n ≥ 3    */
        idx_t k = (n - 1) / 2;             /* k ≥ 1        */
        seq_t val = pow2(k);               /* 2^k          */
        val *= (seq_t)3;
        val -= (seq_t)3;
        return val;
    }
}
