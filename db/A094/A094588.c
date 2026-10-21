/*
 * A094588  a(n) = n*F(n-1) + F(n), where F = A000045.
 * https://oeis.org/A094588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n-1)+F(n) (F = Fibonacci)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 85, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0U)
        return (seq_t)0;

    seq_t f_prev = 0;   /* F(0) */
    seq_t f_curr = 1;   /* F(1) */

    if (n == 1U) {
        /* F(0) = 0, F(1) = 1 */
        return (seq_t)n * f_prev + f_curr;   /* 1*0 + 1 = 1 */
    }

    /* compute Fibonacci numbers up to F(n) */
    for (idx_t i = 2ULL; i <= n; ++i) {
        seq_t next = f_prev + f_curr;  /* F(i) = F(i-2) + F(i-1) */
        f_prev = f_curr;
        f_curr = next;
    }
    /* after loop: f_prev = F(n-1), f_curr = F(n) */
    seq_t Fn_minus1 = f_prev;
    seq_t Fn = f_curr;

    return (seq_t)n * Fn_minus1 + Fn;
}
