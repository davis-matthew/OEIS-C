/*
 * A107008  Primes of the form x^2 + 24*y^2.
 * https://oeis.org/A107008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Enumerates primes of the form x² + 24·y² by testing * successive
 *            numbers ≡ 1 (mod 24). The first such prime is 73. * a(n) returns
 *            the n‑th prime of this form (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 794, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for indices below the offset */

    uint64_t count = 0;
    uint64_t candidate = 73;      /* the first prime of the required form */

    while (true) {
        if (is_prime_uint64(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 24;          /* keep numbers ≡ 1 (mod 24) */
    }
}
