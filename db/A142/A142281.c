/*
 * A142281  Primes congruent to 32 mod 43.
 * https://oeis.org/A142281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 162, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 32 modulo 43:
   a(n) is the nth prime p with p ≡ 32 (mod 43). */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t candidate = 32;   /* first number of the form 43·k + 32 (k = 0) */
    idx_t count = 0;

    while (count < n) {
        candidate += 43;            /* next candidate of the form 43·k + 32 */
        if (is_prime_big(candidate)) {
            ++count;                /* found one of the required primes */
        }
    }
    return candidate;              /* the nth such prime */
}
