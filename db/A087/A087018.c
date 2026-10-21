/*
 * A087018  Row sums of Fibonacci triangle shown below.
 * https://oeis.org/A087018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(T(n)+2) - F(T(n-1)+2) where T(k)=k(k+1)/2 and F is the
 *            Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1; only used for safety */

    /* triangular numbers (fit in idx_t for given n) */
    idx_t t_prev = (idx_t)(n - 1) * ((idx_t)(n - 1) + 1) / 2;
    idx_t t_cur  = (idx_t)n * ((idx_t)n + 1) / 2;

    /* Fibonacci indices */
    idx_t f_prev = t_prev + 2;
    idx_t f_cur  = t_cur  + 2;

    /* Fibonacci values and difference */
    seq_t F_prev = fib(f_prev);
    seq_t F_cur  = fib(f_cur);

    return F_cur - F_prev;
}
