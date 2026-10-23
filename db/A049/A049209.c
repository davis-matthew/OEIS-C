/*
 * A049209  a(n) = -Product_{k=0..n} (7*k-1); sept-factorial numbers.
 * https://oeis.org/A049209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} (7*k-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t factor = (seq_t)(7 * i - 1);
        result *= factor;
    }
    return result;
}
