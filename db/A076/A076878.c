/*
 * A076878  a(n) = ceiling(n^(1/n))^n - n.
 * https://oeis.org/A076878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n == 1; otherwise a(n) = 2^n - n (equivalent to a(n)
 *            = ceil(n^(1/n))^n - n for all n >= 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t pow = pow2(n);          /* pow = 2^n  (works for integer or big int) */
    pow -= (seq_t)n;              /* subtract n */
    return pow;
}
