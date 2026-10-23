/*
 * A117591  a(n) = 2^n + Fibonacci(n).
 * https://oeis.org/A117591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + Fibonacci(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) iteratively using seq_t arithmetic */
    seq_t f_prev = 0;   /* F(0) */
    seq_t f_curr = 1;   /* F(1) */

    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = f_curr;
        f_curr = f_curr + f_prev;
        f_prev = tmp;
    }
    /* After the loop, f_prev holds F(n) */

    return pow2(n) + f_prev;
}
