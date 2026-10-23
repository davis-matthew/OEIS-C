/*
 * A141949  Primes congruent to 2 mod 27.
 * https://oeis.org/A141949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 2 modulo 27 (a(1)=2)
 * Range:     u8 n <= 5, u16 n <= 369, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t candidate = 2;   /* first odd prime that satisfies p ≡ 2 (mod 27) */
    idx_t count    = 0;    /* number of qualifying primes found so far */

    while (true) {
        if ((candidate % 27u) == 2u && is_prime(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 27u;   /* next integer that could be ≡ 2 (mod 27) */
    }
}
