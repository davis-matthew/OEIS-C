/*
 * A142636  Primes congruent to 49 mod 55.
 * https://oeis.org/A142636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142636: primes congruent to 49 mod 55
   The function returns the nth such prime (with offset 1).
   We generate numbers of the form 49 + 55·k that are odd
   (step 110) and test them for primality using the helper
   is_prime_small.  Half the candidates are even and hence
   composite, so we skip them to save work.  */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* offset starts at 1; return 0 for safety */
        return 0;

    idx_t   count = 0;
    seq_t   candidate = 49; /* 49 ≡ 49 (mod 55) but is not prime */

    while (1) {
        if (is_prime_small(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 110;   /* jump to next odd number ≡ 49 (mod 55) */
    }
}
