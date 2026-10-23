/*
 * A119697  a(n) = Fibonacci(n)*n*binomial(2*n,n)/(n+1).
 * https://oeis.org/A119697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n)*n*binomial(2*n,n)/(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    /* binomial(2*n, n) * n / (n+1)  =  Catalan(n) * n  */
    idx_t two_n = n << 1;            /* 2*n (unsigned wrap is OK for valid n) */
    seq_t bin = binomial(two_n, n);  /* binomial(2n, n) */
    seq_t cat = bin / ((seq_t)(n + 1u));  /* Catalan number C(n) */
    seq_t temp = cat * ((seq_t)n);   /* n * Catalan(n)  */
    seq_t fibn = fib(n);             /* Fibonacci(n)    */
    return fibn * temp;              /* final product   */
}
