/*
 * A102273  Primes p such that Q(sqrt(-21p)) has genus characters chi_{-3} =
 *          -1, chi_{-7} = +1.
 * https://oeis.org/A102273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 820, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102273: Primes p such that Q(sqrt(-21p)) has genus characters
   chi_{-3} = -1 and chi_{-7} = +1.  These are exactly the odd primes
   with p ≡ 11, 23, or 71 (mod 84). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count = 0;
    uint64_t candidate = 3; /* start checking from 3 */

    while (1) {
        uint64_t r = candidate % 84ULL;
        if (r == 11ULL || r == 23ULL || r == 71ULL) {
            if (is_prime_u64(candidate)) {
                count++;
                if (count == target)
                    return (seq_t)candidate;
            }
        }
        candidate++;
    }
}
