/*
 * A142579  Primes congruent to 49 mod 53.
 * https://oeis.org/A142579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 49 modulo 53 by enumerating
   candidates of the form 53·k + 49 and testing each for primality. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* defined only for n ≥ 1 */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        idx_t cand = 53 * k + 49;
        if (cand >= 2 && is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++k;
    }
}
