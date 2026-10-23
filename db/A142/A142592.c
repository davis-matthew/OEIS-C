/*
 * A142592  Primes congruent to 29 mod 54.
 * https://oeis.org/A142592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 29 modulo 54
 * Range:     u8 n <= 4, u16 n <= 367, u32 n <= 210, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n<1; return 0 as a sentinel. */
        return 0;
    }

    seq_t candidate = 29;   /* first candidate of the form 54k+29 */
    idx_t  count     = 0;

    while (1) {
        if (is_prime_big(candidate)) {   /* test primality */
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 54;   /* move to the next number ≡29 (mod 54) */
    }
}
