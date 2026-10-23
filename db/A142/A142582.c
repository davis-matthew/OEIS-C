/*
 * A142582  Primes congruent to 52 mod 53.
 * https://oeis.org/A142582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 52 mod 53
 * Range:     u8 n <= 1, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n=0, safeguard */

    idx_t count = 0;                    /* number of primes found so far */
    idx_t candidate = 211;              /* 53*3 + 52  – the first such prime */

    while (true) {
        if (is_prime(candidate)) {       /* test primality of the candidate */
            ++count;
            if (count == n)             /* the nth prime found */
                return (seq_t)candidate;
        }

        candidate += 53;                 /* next number ≡52 (mod 53) */
    }
}
