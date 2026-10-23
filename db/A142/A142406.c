/*
 * A142406  Primes congruent to 25 mod 48.
 * https://oeis.org/A142406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 376, u32 n <= 383, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 25 (mod 48) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1, but guard anyway */

    /* The smallest prime ≡ 25 (mod 48) is 73 */
    uint64_t candidate = 73;
    if (n == 1) return (seq_t)candidate;

    idx_t count = 1;                    /* we already have the first prime */

    for (;;) {
        candidate += 48;                /* iterate only numbers ≡ 25 (mod 48) */
        if (is_prime_big((seq_t)candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
