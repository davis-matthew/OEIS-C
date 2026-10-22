/*
 * A007522  Primes of the form 8*k+7, that is, primes congruent to -1 mod 8.
 * https://oeis.org/A007522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th prime p with p == 7 (mod 8), found by
 *            testing 8*k+7 for k >= 0
 * Range:     u8 n <= 14, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 7;

    for (;;) {
        if (is_prime(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 8;
    }
}
