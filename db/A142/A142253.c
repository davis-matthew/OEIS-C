/*
 * A142253  Primes congruent to 4 mod 43.
 * https://oeis.org/A142253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for primes ≡ 4 (mod 43) – sequence A142253
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below offset – behaviour undefined */

    uint64_t count = 0;
    uint64_t candidate = 47;              /* first prime of the form 43k+4 */

    while (1) {
        if (is_prime_big((seq_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 43;                  /* next number ≡ 4 (mod 43) */
    }
}
