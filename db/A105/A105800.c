/*
 * A105800  Greatest Fibonacci number that is a proper divisor of the n-th
 *          Fibonacci number; a(1) = a(2) = 1.
 * https://oeis.org/A105800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A032742(n)) (greatest Fibonacci proper divisor of
 *            F_n)
 * Range:     u8 n <= 27, u16 n <= 49, u32 n <= 95, u64 n <= 187,
 *            u128 n <= 373, big any n
 * Helpers:   fib_fast, largest_proper_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast.h"
#include "../utils/largest_proper_divisor.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    idx_t d = largest_proper_divisor(n);   /* d = largest proper divisor of n */

    seq_t fib_d, dummy;
    fib_fast(d, &fib_d, &dummy);           /* fib_fast computes F_d and F_{d+1} */
    return fib_d;                          /* a(n) = F_d */
}
