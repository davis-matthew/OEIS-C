/*
 * A073065  a(n) = prime(n) * prime(prime(n)).
 * https://oeis.org/A073065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * prime(prime(n))
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; values for n < 1 are irrelevant. */
    if (n == 0)
        return 0;

    /* First prime: prime(n).  n is 1‑based in OEIS. */
    uint32_t p1 = get_prime(n);

    /* Second prime: prime(p1).  p1 is also 1‑based. */
    uint32_t p2 = get_prime(p1);

    /* Multiply the two primes.  Cast to seq_t to keep the calculation in the
       desired type. */
    return (seq_t)p1 * (seq_t)p2;
}
