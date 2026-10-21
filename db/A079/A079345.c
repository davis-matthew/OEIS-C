/*
 * A079345  a(n) = Fibonacci(n) mod 16.
 * https://oeis.org/A079345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 16. Fast doubling with modular reduction
 *            performed before any subtraction to avoid negative intermediate
 *            results
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   fib_mod_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_mod_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_mod_pair(n, &fn, &fn1);
    return fn;            /* fn is already reduced modulo 16 */
}
