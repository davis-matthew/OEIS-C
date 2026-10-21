/*
 * A072123  Remainder when Fibonacci(n) is divided by prime(n).
 * https://oeis.org/A072123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6583, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod_u32, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = F(n) mod prime(n)
 * (F(n) is the nth Fibonacci number, prime(n) is the nth prime, indexing starts at 1)
 */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/fib_mod_u32.h"

seq_t a(idx_t n)
{
    /* offset is 1, so n >= 1 for valid calls */
    uint32_t p = get_prime(n);
    return fib_mod_u32(n, p);
}
