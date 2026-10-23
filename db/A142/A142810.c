/*
 * A142810  Primes congruent to 12 mod 61.
 * https://oeis.org/A142810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that p ≡ 12 (mod 61)
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime that is congruent to 12 modulo 61.
   n >= 1.  The function works for any build where `seq_t` can hold
   the expected primes (e.g. 64‑bit or arbitrary‑precision). */
seq_t a(idx_t n)
{
    assert(n >= 1);
    idx_t candidate = 73;   /* first prime ≡12 (mod 61) */
    idx_t count    = 1;     /* number of such primes found so far */

    while (count < n) {
        candidate += 61;                /* next number in the progression */
        if (is_prime(candidate)) {
            ++count;
        }
    }
    return (seq_t)candidate;
}
