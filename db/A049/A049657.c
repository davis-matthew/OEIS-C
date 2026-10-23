/*
 * A049657  a(n) = (F(8*n+3) - 2)/3, where F = A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(8*n+3) - 2)/3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where F is the Fibonacci sequence with F(0)=0, F(1)=1. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t idx = n * (idx_t)8 + (idx_t)3;   /* compute 8*n+3 */
    seq_t f = fib(idx);                    /* F(8*n+3) */
    f -= (seq_t)2;                         /* subtract 2 */
    f /= (seq_t)3;                         /* divide by 3 */
    return f;
}
