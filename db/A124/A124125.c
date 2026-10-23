/*
 * A124125  a(n)=(1/(4n))*sum(k=1,n,F(6k)*B(2n-2k)*binomial(2n,2k)) where
 *          F=Fibonacci numbers and B=Bernoulli numbers.
 * https://oeis.org/A124125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(F(6n-3)+4^n*F(2n-1)+2*5^(n-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   fib, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t f1 = fib(6*n-3);         /* F(6n-3)   */
    seq_t f2 = fib(2*n-1);         /* F(2n-1)   */
    seq_t pow4 = pow2(2*n);        /* 4^n       */
    seq_t term2 = pow4 * f2;       /* 4^n * F(2n-1) */
    seq_t pow5_term = power5(n-1) * (seq_t)2;  /* 2*5^(n-1) */
    seq_t sum = f1 + term2 + pow5_term;
    return sum / 4;
}
