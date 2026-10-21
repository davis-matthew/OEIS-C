/*
 * A087385  a(n) = smallest prime == 1 (mod T(n)) where T(n) is the n-th
 *          triangular number (A000217).
 * https://oeis.org/A087385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p ≡ 1 (mod T(n)) * where T(n)
 *            = n*(n+1)/2
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute triangular number in seq_t
       use seq_t arithmetic to avoid overflow */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* candidate primes congruent to 1 mod t */
    seq_t cand = 1 + t;
    while (!is_prime_big(cand))
        cand += t;

    return cand;
}
