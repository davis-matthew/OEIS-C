/*
 * A073588  a(n) = a(n-1)*2^n-1 with a(1)=1.
 * https://oeis.org/A073588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * 2^n - 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, safe fallback */
    seq_t result = 1;              /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        result = result * pow2(i) - 1;   /* recurrence */
    }
    return result;
}
