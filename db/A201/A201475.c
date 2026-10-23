/*
 * A201475  Primes of the form 2n^2 + 7.
 * https://oeis.org/A201475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   this code searches for the n‑th prime of the form 2k^2+7 (OEIS
 *            A201475). It does not use a closed formula, but finds successive
 *            such primes by incrementing k and testing for primality
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 1964, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint64_t count = 0;
    uint64_t k = 0;
    for (;;) {
        /* compute 2*k*k + 7, safe in 64 bits for the required n range */
        uint64_t val = 2ULL * k * k + 7ULL;
        if (is_prime64(val)) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)val;
        }
        ++k;
    }
}
