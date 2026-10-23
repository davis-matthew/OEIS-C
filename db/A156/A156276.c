/*
 * A156276  Denominator of Euler(n, 1/10).
 * https://oeis.org/A156276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 10^n for even n (including n=0), a(1) = 5, a(n) = 10^n /
 *            4 for odd n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);          /* p = 10^n */
    if ((n & 1ULL) == 0) {       /* even n */
        return p;
    } else if (n == 1) {         /* special case n = 1 */
        return (seq_t)5;
    } else {                     /* odd n ≥ 3 */
        return p / (seq_t)4;
    }
}
