/*
 * A031169  a(n) = prime(n+7) - prime(n).
 * https://oeis.org/A031169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+7) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* Compute A031169: difference between the (n+7)th prime and the nth prime.
 * Assumes n is a 1‑based index (n >= 1).  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    return (seq_t)get_prime(n + 7) - (seq_t)get_prime(n);
}
