/*
 * A067988  Row sums of triangle A067330; also of triangle A067418.
 * https://oeis.org/A067988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*((3*n+5)*F(n+1)+(n+1)*F(n))/10, where F is
 *            Fibonacci
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci numbers.               */
    seq_t Fn  = fib(n);
    seq_t Fn1 = fib(n + 1);

    /* Σ = (3*n + 5)*F(n+1) + (n + 1)*F(n)  */
    seq_t term1 = Fn1 * (seq_t)(3 * n + 5);
    seq_t term2 = Fn    * (seq_t)(n + 1);
    seq_t sum   = term1 + term2;

    /* sum is always divisible by 5.   */
    seq_t S = sum / (seq_t)5;

    seq_t n_plus_2 = (seq_t)(n + 2);

    /* a(n) = (n+2)*S/2; divide by 2 before the final multiplication to avoid overflow. */
    seq_t a_val;
    if ((S % (seq_t)2) == 0) {
        a_val = n_plus_2 * (S / (seq_t)2);
    } else {
        a_val = (n_plus_2 / (seq_t)2) * S;
    }
    return a_val;
}
