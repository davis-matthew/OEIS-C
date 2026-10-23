/*
 * A127526  Sequence related to fifth roots of certain Fibonacci fractions.
 * https://oeis.org/A127526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(2n+5) + (n mod 2? 2 : -4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) = F(7)+2, a(2) = F(9)-4, etc. */
    if (n == 0) return 0;            /* undefined for n<1, return 0 */
    idx_t idx = 2*n + 5;             /* index of the Fibonacci number */
    seq_t f = fib(idx);              /* F(idx) */

    if (n & 1)                       /* odd n -> +2 */
        return f + (seq_t)2;
    else                             /* even n -> -4 */
        return f - (seq_t)4;
}
