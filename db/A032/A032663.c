/*
 * A032663  Primes that are the smallest decimal concatenations of n with n +
 *          (0,1,2,3,...).
 * https://oeis.org/A032663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032663: smallest prime formed by concatenating x and
 * x + n-1 for some integer x ≥ 1.  That is,
 *   a(n) = min{ concat(x, x+n-1) such that concat(x, x+n-1) is prime }.
 * The function may be called in any order; it works for all n ≥ 1. */

#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The displacement to use in the concatenation is
       n - 1.  We simply search x = 1, 2, 3, … until a prime is found.
       Both concat_parts and is_prime_big are already provided by the
       build system.                                           */
    idx_t delta = n - 1;               /* difference between the two parts */
    for (idx_t x = 1; ; ++x) {
        /* compute concat(x, x+delta) */
        seq_t val = concat_parts(x, x + delta);
        if (is_prime_big(val))
            return val;
        /* else continue searching */
    }
}
