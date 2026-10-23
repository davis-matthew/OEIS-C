/*
 * A014286  a(n) = Sum_{j=0..n} j*Fibonacci(j).
 * https://oeis.org/A014286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0..n} j * F(j) (F(j) = j-th Fibonacci number)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t f_prev = (seq_t)0;   /* F(0) */
    seq_t f_curr = (seq_t)1;   /* F(1) */
    idx_t j;

    for (j = 0; j <= n; ++j) {
        /* Add j * F(j) to the sum */
        sum += f_prev * (seq_t)j;

        /* Advance Fibonacci numbers */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return sum;
}
