/*
 * A102875  Let f(n) = n+2 if n == 1 mod 3, = n if n == 2 mod 3, = n-2 if n ==
 *          0 mod 3; then a(n) = Fibonacci(f(n)).
 * https://oeis.org/A102875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(f(n)) where * f(n) = n+2 if n ≡ 1 (mod 3), *
 *            f(n) = n if n ≡ 2 (mod 3), * f(n) = n-2 if n ≡ 0 (mod 3) (with
 *            a(0)=0)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* a(0) = 0 */

    idx_t rem = n % 3;
    idx_t f;
    if (rem == 1)
        f = n + 2;
    else if (rem == 2)
        f = n;
    else        /* rem == 0 */
        f = n - 2;

    /* fib_u64 returns seq_t, the appropriate Fibonacci number */
    return fib_u64(f);
}
