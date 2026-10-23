/*
 * A142488  Primes congruent to 20 mod 51.
 * https://oeis.org/A142488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 185, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 20 (mod 51) */
/* Computes the n-th prime p such that p % 51 == 20, where n >= 1. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1: a(1) is the first term */
    idx_t count = 0;                  /* how many qualifying primes found */
    idx_t k = 0;                      /* iterator for numbers of form 51*k + 20 */
    seq_t candidate;

    for (;; ++k) {
        candidate = (seq_t)(51 * k + 20);   /* construct the next candidate */
        if (is_prime_big(candidate)) {      /* check primality */
            ++count;
            if (count == n) return candidate;   /* nth prime found */
        }
    }
}
