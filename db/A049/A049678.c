/*
 * A049678  a(n) = F(8*n+4)/3, where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(8*n+4)/3, where F is the Fibonacci sequence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t val = fib(8*n + 4);   /* F(8n+4) */
    val /= 3;                   /* exact division */
    return val;
}
