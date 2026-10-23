/*
 * A176997  Integers k such that 2^(k-1) == 1 (mod k).
 * https://oeis.org/A176997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6606, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176997: return the n-th integer k such that 2^(k‑1) ≡ 1 (mod k).
   The sequence starts with 1, then includes all odd primes and all odd
   base‑2 pseudoprimes.  The implementation simply scans odd integers and
   tests the congruence using the supplied modular‑exponentiation and prime
   test routines. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range, never requested by OEIS */

    /* a(1) = 1 by definition. */
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t count = 1;          /* we already have term a(1) = 1 */
    idx_t k = 3;              /* start testing with the first odd integer >1 */

    while (count < n) {
        if (is_prime(k) || mod_pow((seq_t)2, k-1, (seq_t)k) == (seq_t)1) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k += 2;               /* advance to the next odd integer */
    }

    /* This point is never reached for n >= 1. */
    return (seq_t)0;
}
