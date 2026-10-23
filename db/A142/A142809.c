/*
 * A142809  Primes congruent to 11 mod 61.
 * https://oeis.org/A142809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th prime congruent to 11 (mod 61).  The sequence is A142809. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* Undefined for n<1, return 0 */
    }

    uint64_t prime = 11;      /* first prime ≡ 11 mod 61 */
    idx_t count = 1;

    if (n == 1) {
        return (seq_t)prime;
    }

    /* Search forward in the arithmetic progression 61k+11 */
    while (count < n) {
        prime += 61;
        if (is_prime(prime)) {
            ++count;
        }
    }

    return (seq_t)prime;
}
