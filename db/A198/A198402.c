/*
 * A198402  a(n) = 5^n * n^5.
 * https://oeis.org/A198402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * n^5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 43,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t pow5n = power5(n);          /* 5^n */
    seq_t base   = (seq_t)n;          /* n */
    seq_t n5 = base;
    n5 *= base; /* n^2 */
    n5 *= base; /* n^3 */
    n5 *= base; /* n^4 */
    n5 *= base; /* n^5 */
    return pow5n * n5;
}
