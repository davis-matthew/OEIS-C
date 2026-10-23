/*
 * A117867  Number of palindromes (in base 8) below 8^n.
 * https://oeis.org/A117867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*8^((n-1)/2)-2 for odd n, a(n) = 2*8^(n/2)-2 for even n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                     /* undefined for n < 1, return 0 */
    if (n & 1u) {                     /* n is odd */
        idx_t k   = (n - 1u) >> 1u;    /* (n-1)/2 */
        idx_t exp = 3u * k;            /* exponent for 2^exp = 8^k */
        seq_t v   = pow2(exp);         /* 2^(3k) */
        v = v * (seq_t)9;              /* 9*8^k */
        v -= (seq_t)2;                 /* subtract 2 */
        return v;
    } else {                          /* n is even */
        idx_t k   = n >> 1u;           /* n/2 */
        idx_t exp = 3u * k;            /* exponent for 2^exp = 8^k */
        seq_t v   = pow2(exp);         /* 2^(3k) */
        v = v * (seq_t)2;              /* 2*8^k */
        v -= (seq_t)2;                 /* subtract 2 */
        return v;
    }
}
