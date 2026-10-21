/*
 * A079285  First differences of A079284.
 * https://oeis.org/A079285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fib(n+2) - (n even ? 2^(n/2) : 0)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = fib(n + 2);
    if ((n & 1) == 0)
        v -= pow2(n / 2);
    return v;
}
