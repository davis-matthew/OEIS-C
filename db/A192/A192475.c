/*
 * A192475  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^(n+1)+x^(2n).
 * https://oeis.org/A192475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(2n) + Fib(n+1) (Fib = Fibonacci numbers from A000045,
 *            Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute Fib(2n) and Fib(n+1) and return their sum. */
    seq_t f2n = fib(n << 1);   /* 2n fits in idx_t for the intended input range */
    seq_t fnp1 = fib((n + 1) & ((idx_t)-1));
    return f2n + fnp1;
}
