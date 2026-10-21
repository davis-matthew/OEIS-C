/*
 * A086361  a(n)=A085956(3n).
 * https://oeis.org/A086361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 11, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest prime p such that
 *   p is prime,
 *   (p-1)/(6n) is prime,
 *   and 6n*p + 1 is prime.
 * This is equivalent to a(n) = A085956(3n).
 *
 * Implementation: iterate over primes q = (p-1)/(6n), compute
 *   p = 6n * q + 1,
 *   test p and 6n*p+1 for primality.
 * The first q producing both primes yields a(n).
 * The routine uses is_prime_big for arbitrary‑precision
 * primality checking (works for 64‑bit and larger builds).
 */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Ensure n >= 1 (offset 1) */
    if (n == 0) return 0;

    /* tn = 6 * n  (type seq_t to avoid overflow) */
    seq_t tn = (seq_t)n * 6;

    /* Iterate over prime q */
    for (uint64_t q = 2; ; ++q) {
        if (!is_prime_big((seq_t)q))
            continue;               /* q must be prime */

        /* Compute p = tn * q + 1 */
        seq_t p = tn * (seq_t)q + 1;
        if (!is_prime_big(p))
            continue;               /* p must be prime */

        /* Check that 6n * p + 1 is prime */
        seq_t r = tn * p + 1;
        if (is_prime_big(r))
            return p;               /* Found the required prime */
    }
}
