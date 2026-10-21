/*
 * A072352  a(n) is the largest n-digit Fibonacci number.
 * https://oeis.org/A072352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest Fibonacci number with exactly n decimal digits.
   Search by generating successive Fibonacci numbers until the next
   one would exceed 10^n, then return the previous value. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n is the first number with n+1 digits. */
    seq_t limit = pow10(n);

    seq_t f_prev = 0;           /* F(0)   */
    seq_t f_curr = 1;           /* F(1)   */
    seq_t best   = 0;           /* largest <= n digits found so far */

    while (f_curr < limit) {
        best = f_curr;
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    return best;
}
