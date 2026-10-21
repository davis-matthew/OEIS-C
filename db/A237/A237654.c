/*
 * A237654  G.f.: exp( Sum_{n>=1} 5*Fibonacci(n-1)*Fibonacci(n+1) * x^n/n ).
 * https://oeis.org/A237654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*Lucas(2*n+5) + (28+25*n+5*n^2)*(-1)^n)/50 (Lucas
 *            sequence: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Lucas(2*n+5) = fib(2*n+4) + fib(2*n+6) */
    idx_t k = 2*n + 5;
    seq_t fib_k_minus1 = fib(k - 1);   /* fib(2*n+4) */
    seq_t fib_k_plus1  = fib(k + 1);   /* fib(2*n+6) */
    seq_t luc         = fib_k_minus1 + fib_k_plus1;

    /* First part of numerator: 2*Lucas(2*n+5) */
    seq_t term1 = luc * ((seq_t)2);

    /* Polynomial part: 28 + 25*n + 5*n^2 */
    seq_t poly = ((seq_t)28);
    poly += ((seq_t)25) * ((seq_t)n);
    seq_t n_sq = ((seq_t)n) * ((seq_t)n);
    poly += ((seq_t)5) * n_sq;

    /* Combine with (-1)^n */
    seq_t numerator;
    if ((n & 1) == 0)          /* n even */
        numerator = term1 + poly;
    else                        /* n odd  */
        numerator = term1 - poly;

    /* Final division by 50 */
    return numerator / ((seq_t)50);
}
