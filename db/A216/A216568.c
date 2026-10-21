/*
 * A216568  Smallest k such that prime(n)*k-1 is prime.
 * https://oeis.org/A216568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that prime(n)*k - 1 is prime
 * Range:     u8 n <= 13, u16 n <= 1594, u32 n <= 1594, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    seq_t p = (seq_t)get_prime(n);
    /* Search for the smallest k with pk−1 prime. */
    for (seq_t k = 1;; ++k) {
        seq_t val = p * k - (seq_t)1;
        if (is_prime_big(val))
            return k;
    }
}
