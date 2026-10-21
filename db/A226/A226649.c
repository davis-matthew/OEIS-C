/*
 * A226649  Fibonacci shuffles: a(2n) = A000071(n) and a(2n+1) = A001611(n+2).
 * https://oeis.org/A226649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = fib(n+1) - 1 Formula: a(2n+1) = fib(n+2) + 1 (fib(k) is
 *            the standard Fibonacci number with F(0)=0, F(1)=1)
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 92, u64 n <= 184,
 *            u128 n <= 370, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                     /* even index: n = 2k */
        idx_t k = n >> 1;
        seq_t f = fib(k + 1);               /* F(k+1) */
        return f - 1;
    } else {                                /* odd index:  n = 2k+1 */
        idx_t k = (n - 1) >> 1;             /* k = (n-1)/2 */
        seq_t f = fib(k + 2);               /* F(k+2) */
        return f + 1;
    }
}
