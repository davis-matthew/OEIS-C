/*
 * A206611  Fibonacci sequence beginning 13, 7.
 * https://oeis.org/A206611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*Fibonacci(n) - 6*Fibonacci(n-1) Recurrence: a(n) =
 *            a(n-1) + a(n-2), a(1) = 13, a(2) = 7
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /*  Handle the offset explicitly. The behavior for n < 1 is
        irrelevant; we just return 0 to avoid UB. */
    if (n == 0) return 0;
    if (n == 1) return 13;
    if (n == 2) return 7;

    /*  Use the fast‑doubling Fibonacci implementation from utils.
        F(n) is defined with F(0)=0, F(1)=1.  */
    seq_t fn  = fib(n);      /* Fibonacci(n)   */
    seq_t fn1 = fib(n - 1);  /* Fibonacci(n-1) */

    return 13 * fn - 6 * fn1;
}
