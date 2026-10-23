/*
 * A142893  Primes congruent to 8 mod 63.
 * https://oeis.org/A142893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 8 mod 63 by iterating candidates. */
#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 1;
    idx_t candidate = 71;
    if (n == 1) return (seq_t)candidate;

    while (count < n) {
        candidate += 63;
        if (is_prime(candidate))
            ++count;
    }
    return (seq_t)candidate;
}
