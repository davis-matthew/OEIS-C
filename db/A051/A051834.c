/*
 * A051834  a(n) = Fibonacci(Pn-1) mod Pn, where Pn is the n-th prime.
 * https://oeis.org/A051834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(Pn-1) mod Pn where Pn is the n-th prime (Pn =
 *            get_prime(n))
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   fib_mod_u32, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fib_mod_u32.h"

seq_t a(idx_t n)
{
    /* get nth prime (offset 1) */
    uint32_t p = get_prime(n);   /* p fits in 32 bits for n <= 10000 */
    /* compute Fibonacci(p-1) mod p */
    return fib_mod_u32((idx_t)(p - 1), p);
}
