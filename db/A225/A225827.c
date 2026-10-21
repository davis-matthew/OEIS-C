/*
 * A225827  Number of binary pattern classes in the (3,n)-rectangular grid:
 *          two patterns are in same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A225827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=24. For even n=2m (m≥2): a(n) = 2^(3m-1) *
 *            ( 2^(3m-1)+2^(m-1)+1 ) For odd n=2m+1 (m≥1): a(n) = 2^(3m) * (
 *            2^(3m+1)+2^m+3 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)24;

    if ((n & 1) == 0) {            /* even n = 2m, m >= 2 */
        idx_t m = n >> 1;
        seq_t powA = pow2((idx_t)(3 * m - 1));   /* 2^(3m-1) */
        seq_t powB = pow2((idx_t)(m - 1));       /* 2^(m-1)   */
        seq_t sum  = powA + powB + (seq_t)1;
        return powA * sum;                     /* 2^(3m-1) * ( … ) */
    } else {                           /* odd n = 2m+1, m >= 1 */
        idx_t m = (n - 1) >> 1;
        seq_t powA = pow2((idx_t)(3 * m));      /* 2^(3m)     */
        seq_t powB = pow2(m);                   /* 2^m        */
        seq_t powC = pow2((idx_t)(3 * m + 1));  /* 2^(3m+1)   */
        seq_t sum  = powC + powB + (seq_t)3;
        return powA * sum;                     /* 2^(3m) * ( … ) */
    }
}
