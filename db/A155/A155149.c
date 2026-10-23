/*
 * A155149  Numbers k such that k^4 has exactly 3 different decimal digits.
 * https://oeis.org/A155149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pattern: a(1..8) = 4,5,9,11,16,20,30,34; for n ≥ 9,
   let k = n-9, m = floor(k/2)+2.
   If k is even, a(n) = 2·10^m; if k is odd, a(n) = 3·10^m. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Small exceptional block */
    if (n <= 8) {
        static const seq_t small[] = {4, 5, 9, 11, 16, 20, 30, 34};
        return small[n-1];
    }

    /* For n ≥ 9: 2·10^m and 3·10^m with m increasing from 2. */
    idx_t k = n - 9;                 /* index starting at 0 */
    idx_t m = (k / 2) + 2;           /* exponent for 10 */
    seq_t base = pow10(m);           /* 10^m */

    if ((k & 1) == 0)                /* even positions -> 2·10^m */
        return base * 2;
    else                             /* odd positions -> 3·10^m */
        return base * 3;
}
