/*
 * A142852  Primes congruent to 54 mod 61.
 * https://oeis.org/A142852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 54 modulo 61
 * Range:     u8 none, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* offset handling, not part of the sequence */

    idx_t  found = 0;
    idx_t  k     = 0;
    seq_t  candidate;

    while (true) {
        /* numbers of the form 61*k + 54 */
        candidate = (seq_t)61 * (seq_t)k + (seq_t)54;
        if (isPrimeTrial(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++k;
    }
}
