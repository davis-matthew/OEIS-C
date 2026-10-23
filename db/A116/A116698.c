/*
 * A116698  Expansion of (1-x+3*x^2+x^3) / ((1-x-x^2)*(1+2*x^2)).
 * https://oeis.org/A116698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fib(n+1) - (-1)^⌊(n-1)/2⌋ * (n mod 2) * 2^⌊n/2⌋ Initial
 *            values: a(0)=1, a(1)=0, a(2)=2, a(3)=5 Computation uses fast
 *            Fibonacci and power-of‑two functions
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[4] = {1, 0, 2, 5};

    if (n < 4)
        return init[n];

    seq_t f = fib(n + 1);               /* Fib(n+1)   */
    if ((n & 1) == 0)                   /* n is even  */
        return f;                       /* a(n) = Fib(n+1) */

    /* n is odd  */
    idx_t k = n >> 1;                   /* ⌊n/2⌋      */
    seq_t p2 = pow2(k);                 /* 2^k        */
    if ((k & 1) == 0)                   /* (-1)^k =  1  */
        return f - p2;                  /* subtract 2^k */
    else                                 /* (-1)^k = -1  */
        return f + p2;                  /* add     2^k */
}
