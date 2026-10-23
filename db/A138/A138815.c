/*
 * A138815  Divisors of 16775168 (half the 5th perfect number).
 * https://oeis.org/A138815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = 2^(n-1) for n<=12, a(n) = 2^n - 2^(n-13) for n>12
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but safeguard for n=0 */
    if (n <= 12)
        return pow2(n - 1);        /* a(n) = 2^(n-1) for n <= 12 */
    else
        return pow2(n) - pow2(n - 13); /* a(n) = 2^n - 2^(n-13) for n > 12 */
}
