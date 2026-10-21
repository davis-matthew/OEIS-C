/*
 * A072063  Smallest prime of form prime(n)+k*n, k>0.
 * https://oeis.org/A072063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1216, u32 n <= 4670, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest prime of the form prime(n)+k*n, k>0 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n < 1 return 0. */
    if (n < (idx_t)1) {
        return (seq_t)0;
    }

    /* The n-th prime (1‑based). */
    seq_t p = (seq_t)get_prime((idx_t)n);

    /* Modulus n as a seq_t. */
    seq_t mod = (seq_t)n;

    /* Search for the smallest k ≥ 1 such that p + k*n is prime. */
    idx_t k = 1;
    for (;;) {
        /* Compute candidate = p + k*mod; all arithmetic is done in seq_t. */
        seq_t prod     = (seq_t)mod * (seq_t)k;
        seq_t candidate = p + prod;

        if (is_prime_big(candidate)) {
            return candidate;
        }
        ++k;
    }
}
