/*
 * A129982  Fibonacci numbers sandwiched between 1's.
 * https://oeis.org/A129982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 1; a(2*n+1) = Fib(n) (fib(0)=0, fib(1)=1)
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-01)
 * Helpers:   fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fibonacci.h"

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)          /* even n */
        return (seq_t)1;
    else                               /* odd n */
        return fibonacci((n - 1) / 2);
}
