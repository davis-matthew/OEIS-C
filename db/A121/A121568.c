/*
 * A121568  a(n) = Fibonacci((prime(n)-1)/2), n>1.
 * https://oeis.org/A121568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci((prime(n)-1)/2) for n>1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* behavior for n < offset is irrelevant */

    /* Get the nth prime (1‑indexed).  get_prime returns a 32‑bit value. */
    idx_t p = (idx_t)get_prime(n);

    /* Compute k = (p - 1) / 2, the Fibonacci index. */
    idx_t k = (p - 1) / 2;

    /* Return the Fibonacci number F_k. */
    return fib(k);
}
