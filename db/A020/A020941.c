/*
 * A020941  Main diagonal of Wythoff array: w(n,n)=[ n*tau ]F(n+1)+(n-1)F(n),
 *          where tau=(1+sqrt(5))/2, F(n) = Fibonacci numbers.
 * https://oeis.org/A020941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*tau)*F(n+1) + (n-1)*F(n), where tau =
 *            (1+sqrt(5))/2 and F(k) are Fibonacci numbers. Indexing starts at
 *            n = 1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) /* sequence is defined for n >= 1 */
        return (seq_t)0;

    /* Fibonacci numbers for indices n and n+1 */
    seq_t F_n   = fib(n);
    seq_t F_n1  = fib(n + 1);

    /* n as seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* floor(sqrt(5 * n^2)) gives floor(n * sqrt(5)) */
    seq_t tmp = (seq_t)5 * n_seq * n_seq;
    seq_t sqrt5 = floor_sqrt(tmp);

    /* floor(n * tau) = (n + floor(n*sqrt5)) / 2 */
    seq_t k = (n_seq + sqrt5) / (seq_t)2;

    /* (n-1) as seq_t */
    seq_t n_minus1 = (seq_t)(n - 1);

    /* Compute the two terms of the formula */
    seq_t term1 = k * F_n1;
    seq_t term2 = n_minus1 * F_n;

    return term1 + term2;
}
