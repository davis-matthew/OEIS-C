/*
 * A032625  Primes that are concatenations of n with n + 2.
 * https://oeis.org/A032625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th prime that can be written as the
 * concatenation of two consecutive odd integers.  The sequence
 * starts 13, 79, 911, ... as given in OEIS A032625.  This
 * implementation simply searches odd integers progressively
 * checking primeness of the concatenated number.  No caching is
 * required; each call is evaluated independently. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    /* a(1) = first term, offset is 1; return 0 for n==0 (won't call). */
    if (n == 0) return 0;

    idx_t count = 0;      /* number of primes found so far */
    idx_t odd   = 1;      /* smallest odd integer to try */

    for (;; odd += 2) {   /* loop over all odd integers: 1,3,5,... */
        seq_t val = concat_parts(odd, odd + 2);   /* m || (m+2)  */
        if (is_prime_big(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
    }
}
