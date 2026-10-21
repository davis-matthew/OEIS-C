/*
 * A065451  a(n) = Fibonacci(phi(n)), a(0) = 0.
 * https://oeis.org/A065451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A000010(n)), a(0)=0
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 52, u64 n <= 96, u128 n <= 190,
 *            big any n
 * Helpers:   fib, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0;

    /* φ(n) always fits in idx_t for n ≤ 10 000 */
    seq_t phi_val = phi(n);
    idx_t fib_idx = (idx_t)phi_val;
    return fib(fib_idx);
}
