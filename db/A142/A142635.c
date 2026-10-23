/*
 * A142635  Primes congruent to 48 mod 55.
 * https://oeis.org/A142635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 121, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime congruent to 48 modulo 55 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t k = 0;

    while (len < n) {
        idx_t val = 48 + 55 * k;
        seq_t candidate = (seq_t)val;

        if (candidate > 2 &&
            (candidate % 5) != 0 &&
            (candidate % 11) != 0 &&
            is_prime_big(candidate)) {
            append_candidate(&cache, &len, &cap, candidate);
        }
        ++k;
    }

    return cache[n - 1];
}
