/*
 * A142145  Primes congruent to 36 mod 37.
 * https://oeis.org/A142145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth prime ≡ 36 (mod 37) by linear search.
   Uses the helper is_prime_small(seq_t) from the utils library. */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined, but safe return for n < 1 */

    seq_t candidate = 73;    /* first prime congruent to 36 mod 37 */
    idx_t   count    = 0;

    while (1) {
        if (is_prime_small(candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate += 37;     /* next number with residue 36 mod 37 */
    }
}
