/*
 * A133586  Expansion of x*(1+2*x)/( (x^2-x-1)*(x^2+x-1) ).
 * https://oeis.org/A133586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1) if n is odd, a(n) = 2·F(n) if n is even; where
 *            F(k) is the Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   _fib_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_fib_calc.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* safety: offset starts at 1 */
    if (n & 1) {                       /* odd n: a(n) = F(n+1) */
        return _fib_calc(n + 1);
    } else {                           /* even n: a(n) = 2·F(n) */
        return _fib_calc(n) * (seq_t)2;
    }
}
