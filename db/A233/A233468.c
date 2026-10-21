/*
 * A233468  The digital root of prime(n+1) minus the digital root of prime(n).
 * https://oeis.org/A233468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, easy
 * Formula:   a(n) = prime(n+1) mod 9 - prime(n) mod 9
 * Range:     i8 n <= 30, i16 n <= 6663, i32 n <= 9234, i64 n <= 9234,
 *            i128 n <= 9234, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the nth and (n+1)th primes (1‑based) */
    uint32_t p_n  = get_prime(n);
    uint32_t p_n1 = get_prime(n + 1);

    /* compute the digital root via mod 9 difference */
    seq_t r1 = (seq_t)p_n1 % 9;
    seq_t r0 = (seq_t)p_n % 9;
    return r1 - r0;
}
