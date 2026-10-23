/*
 * A142037  Primes congruent to 7 mod 32.
 * https://oeis.org/A142037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 7 (mod 32)
 * Range:     u8 n <= 5, u16 n <= 335, u32 n <= 405, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset is 1 – behaviour for n<1 is irrelevant */

    idx_t found = 0;                      /* number of qualifying primes found so far */
    idx_t k     = 0;                      /* index of the current candidate (7 + 32*k) */
    seq_t result = 0;                     /* holds the value of the nth prime */

    while (found < n) {
        seq_t cand = (seq_t)7;             /* candidate number */
        cand += (seq_t)32 * (seq_t)k;     /* compute 7 + 32*k */

        /* primality check */
        if (is_prime_big(cand)) {
            ++found;
            result = cand;
        }

        ++k;
    }

    return result;
}
