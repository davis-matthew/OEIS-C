/*
 * A142122  Primes congruent to 13 mod 37.
 * https://oeis.org/A142122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142122: primes congruent to 13 modulo 37.  
   The function scans numbers of the form 13 + 37·k and tests each
   for primality until the nth such prime is found. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(0) is unused; offset 1 */
    if (n == 0) return 0;

    idx_t count = 0;          /* number of primes found so far   */
    idx_t candidate = 13;     /* first number ≡ 13 (mod 37)      */

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 37;      /* next number ≡ 13 (mod 37)      */
    }
}
