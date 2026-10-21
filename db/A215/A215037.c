/*
 * A215037  a(n) = Sum_{k=0..n} fibonomial(k+3,3), n >= 0.
 * https://oeis.org/A215037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215037: a(n) = sum_{k=0..n} fib(k+3)*fib(k+2)*fib(k+1)/2
   Formula used: a(n) = (F(n+3)^2*F(n+2) + (-1)^n*F(n+1) - 1) / 4
   where F(m) = fib(m).  */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci numbers needed for the closed form.   */
    seq_t fn1 = fib(n + 1);   /* F(n+1) */
    seq_t fn2 = fib(n + 2);   /* F(n+2) */
    seq_t fn3 = fib(n + 3);   /* F(n+3) */

    /* numerator: F(n+3)^2 * F(n+2)  +/-  F(n+1)  - 1   */
    seq_t num = fn3 * fn3 * fn2;          /* F(n+3)^2 * F(n+2) */
    if ((n & 1) == 0)                     /* (-1)^n is +1 for even n   */
        num += fn1;                       /* add  F(n+1)                */
    else
        num -= fn1;                       /* subtract F(n+1)            */
    num -= 1;                              /* subtract the final 1       */

    /* division by 4 is exact for all n >= 0          */
    return num / 4;
}
