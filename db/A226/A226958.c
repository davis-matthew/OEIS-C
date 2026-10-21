/*
 * A226958  a(n) = Fibonacci(n-2)*Fibonacci(n)*Fibonacci(n+2).
 * https://oeis.org/A226958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n-2)*Fibonacci(n)*Fibonacci(n+2) Alternative
 *            efficient formula used: a(n) = (Fibonacci(3*n) - 8*(-1)^n *
 *            Fibonacci(n)) / 5 (OEIS A226958) Verification for first terms:
 *            a(1)=2, a(2)=0, a(3)=10, a(4)=24, … The implementation uses the
 *            fast‑doubling fib routine from "../utils/fib.h"
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) and Fibonacci(3*n). */
    seq_t fibn   = fib(n);
    seq_t fib3n  = fib(n * (idx_t)3);

    /* 8 * Fibonacci(n) */
    seq_t eight_fibn = fibn * (seq_t)8;

    /* According to the sign of (-1)^n: 
       even n -> subtract, odd n  -> add. */
    seq_t term;
    if ((n & (idx_t)1ULL) == 0) {   /* n even */
        term = fib3n - eight_fibn;
    } else {                         /* n odd */
        term = fib3n + eight_fibn;
    }

    /* Divide by 5, always exact for integer n >= 1. */
    return term / (seq_t)5;
}
