/*
 * A093308  a(n) = Fibonacci(prime(prime(n))).
 * https://oeis.org/A093308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(prime(prime(n)))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute Fibonacci of the prime of the prime at position n */
    idx_t p1 = get_prime(n);         /* prime(n)          */
    idx_t p2 = get_prime(p1);        /* prime(prime(n))   */
    return fib(p2);                  /* Fibonacci(p2)     */
}
