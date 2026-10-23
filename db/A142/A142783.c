/*
 * A142783  Primes congruent to 56 mod 59.
 * https://oeis.org/A142783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 56 modulo 59 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, but guard against 0 */

    idx_t count = 0;                   /* how many primes found */
    idx_t k = 0;                       /* index for candidate primes */

    while (1) {
        ++k;                           /* start with k = 1 */
        idx_t candidate = 59 * k + 56;
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
