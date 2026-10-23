/*
 * A049662  a(n) = (F(6*n+2)-1)/4, where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(6*n+2)-1)/4, where F(k) is the Fibonacci number
 *            (F(0)=0, F(1)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n) {
    idx_t k = 6 * n + 2;          /* argument for the Fibonacci function */
    seq_t f = fib(k);             /* F(6*n+2) */
    return (f - 1) / 4;           /* (F(6*n+2)-1)/4 */
}
