/*
 * A142369  Primes congruent to 18 mod 47.
 * https://oeis.org/A142369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime that is congruent to 18 modulo 47 (search by trial). */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 – return 0 */

    idx_t candidate = 18;                 /* first integer ≡ 18 (mod 47) */
    idx_t count = 0;

    while (1) {
        if (is_prime(candidate)) {         /* test primality of candidate */
            ++count;
            if (count == n) {
                return (seq_t)candidate;   /* convert to seq_t before returning */
            }
        }
        candidate += 47;                   /* next number ≡ 18 (mod 47) */
    }
}
