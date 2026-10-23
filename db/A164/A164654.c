/*
 * A164654  a(n) = 2*a(n-2) for n > 2; a(1) = 3, a(2) = 8.
 * https://oeis.org/A164654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n>2; a(1)=3, a(2)=8 Equivalently: if n is
 *            odd: a(n) = 3 * 2^((n-1)/2) if n is even: a(n) = 2^(n/2 + 2)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* outside the defined offset */
    if ((n & 1) == 1) {                   /* odd n */
        idx_t exp = (n - 1) >> 1;          /* (n-1)/2 */
        return pow2(exp) * (seq_t)3;
    } else {                               /* even n */
        idx_t exp = (n >> 1) + 2;          /* n/2 + 2 */
        return pow2(exp);
    }
}
