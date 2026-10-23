/*
 * A154691  Expansion of (1 + x + x^2) / ((1 - x)*(1 - x - x^2)).
 * https://oeis.org/A154691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*fib(n+3) - 3
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    seq_t f = fib(n + 3);     /* f = Fib(n+3) */
    f = mul_small(f, 2);       /* f = 2 * Fib(n+3) */
    f -= (seq_t)3;             /* f = 2*Fib(n+3) - 3 */
    return f;
}
