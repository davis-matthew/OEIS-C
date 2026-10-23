/*
 * A167441  Prime numbers ending in the prime number 71.
 * https://oeis.org/A167441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the n‑th prime that ends with the two‐digit
   pattern 71 (i.e. a(n) ≡ 71 (mod 100), with a(1)=71).  The function
   iterates over candidates of the form 100·k + 71, tests them for
   primality using the supplied helper is_prime_big, and returns the
   n‑th prime found.  Since the helper takes a seq_t argument, the
   candidate is constructed and tested in the seq_t type. */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  For n = 0 the result is undefined;
       returning 0 avoids an out‑of‑bounds read. */
    if (n == 0) return 0;

    idx_t found = 0;          /* how many qualifying primes seen so far */
    idx_t k     = 0;          /* index of current candidate: 100·k + 71 */

    while (1) {
        seq_t candidate = (seq_t)100 * (seq_t)k + (seq_t)71;

        if (is_prime_big(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        ++k;
    }
}
