/*
 * A056570  Third power of Fibonacci numbers (A000045).
 * https://oeis.org/A056570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(n))^3 where Fibonacci(0)=0, Fibonacci(1)=1 and
 *            Fibonacci(n)=Fibonacci(n-1)+Fibonacci(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   fib_fast_doubling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t if needed (seq_t may be bigger) */
#include "../utils/fib_fast_doubling.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_fast_doubling(n, &fn, &fn1);              /* fn = F(n) */
    return fn * fn * fn;                         /* cube of the Fibonacci number */
}
