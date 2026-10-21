/*
 * A216898  a(n) = smallest number k such that both k - n^2 and k + n^2 are
 *          primes.
 * https://oeis.org/A216898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that (k - n^2) and (k + n^2) are primes
 * Range:     u8 n <= 10, u16 n <= 246, u32 n <= 50, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below searches for the least integer k satisfying the
 * condition by brute force, testing two primality values for each
 * candidate.  It works for any idx_t and seq_t pair provided by the
 * build system, relying on the is_prime_big function defined in the
 * utils library.  The algorithm runs in O(expected_gap * log k)
 * time, which is well below the required limit for n up to 10,000. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 in seq_t form */

    seq_t k = sq + (seq_t)2;                /* minimal candidate (k ≥ n^2 + 2) */
    for (;; ++k) {
        seq_t p1 = k - sq;                  /* k - n^2 */
        seq_t p2 = k + sq;                  /* k + n^2 */

        /* Both p1 and p2 must be primes.  The primality routine will
         * automatically treat values ≤ 1 as non‑prime. */
        if (is_prime_big(p1) && is_prime_big(p2))
            return k;
    }
}
