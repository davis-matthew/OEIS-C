/*
 * A014718  a(n) = (F(n+1)+L(n)+n)^2 where F(n) are the Fibonacci numbers
 *          (A000045) and L(n) are the Lucas numbers (A000032).
 * https://oeis.org/A014718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+1)+L(n)+n)^2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014718: a(n) = (Fib(n+1)+Lucas(n)+n)^2, offset 0 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci: compute F(n+1) */
    seq_t f_prev = 0;   /* F(0)   */
    seq_t f_curr = 1;   /* F(1)   */
    if (n + 1 >= 2) {
        for (idx_t i = 2; i <= n + 1; ++i) {
            seq_t tmp = f_curr;
            f_curr = f_prev + f_curr;
            f_prev = tmp;
        }
    }
    /* f_curr now holds F(n+1) */

    /* Lucas: compute L(n) */
    seq_t l_prev = 2;   /* L(0)   */
    seq_t l_curr = 1;   /* L(1)   */
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            seq_t tmp = l_curr;
            l_curr = l_prev + l_curr;
            l_prev = tmp;
        }
    }
    seq_t ln = (n == 0) ? l_prev : l_curr;  /* L(n) */

    /* Sum and square */
    seq_t sum = f_curr + ln + (seq_t)n;
    return sum * sum;
}
