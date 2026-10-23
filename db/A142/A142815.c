/*
 * A142815  Primes congruent to 17 mod 61.
 * https://oeis.org/A142815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 17 (mod 61) – by iterating candidates */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* offset = 1; a(1) = 17 */
    if (n == 0)
        return 0; /* unused values can be 0 */

    idx_t  found = 0;
    uint64_t candidate = 17;

    while (1)
    {
        if (is_prime((idx_t)candidate))
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        candidate += 61;
    }
}
