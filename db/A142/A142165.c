/*
 * A142165  Primes congruent to 10 mod 39.
 * https://oeis.org/A142165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 266, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return the n‑th prime congruent to 10 modulo 39 (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n < 1 */

    idx_t count = 0;
    idx_t k     = 0;             /* k = 0 gives candidate = 10 */

    while (true) {
        idx_t candidate = 39 * k + 10;   /* numbers ≡10 (mod 39) */

        if (candidate >= 2 && is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++k;
    }
}
