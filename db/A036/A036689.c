/*
 * A036689  Product of a prime and the previous number.
 * https://oeis.org/A036689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (prime(n)-1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

/* a(n) returns the product of the n‑th prime and one less than it.
   The index n is 1‑based, as per the OEIS offset. */
seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);   /* nth prime (1‑based) */
    return p * (p - 1);              /* p * (p-1) */
}
